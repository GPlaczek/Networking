#include "client.hpp"
#include "room.hpp"
#include "log.hpp"
#include "words.hpp"

#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

void Room::initGame() {
    this->game = new struct game;
    this->game->round_num = 0;
    this->game->seconds_left = 0;
}

Room::Room(int maxPlayers, int nRounds, int roundTime) {
    this -> maxPlayers = maxPlayers;
    this -> nRounds = nRounds;
    this -> roundTime = roundTime;
    this -> describer = -1;
    this -> nPlayers = 0;
    this -> epollFd = epoll_create(maxPlayers);
    this -> players = {};
    this -> clock_inevent = NULL;
    this -> stop = false;

    int pp[2];
    pipe2(pp, O_DIRECT);
    this -> __pipeRead = pp[0];
    this -> pipeWrite = pp[1];
    pipe2(pp, O_DIRECT);
    this -> pipeRead = pp[0];
    this -> __pipeWrite = pp[1];
}

Room::~Room() {
    PPRINTF(this->logger, YELLOW, "Destructing");
    close(this -> epollFd);
    close(this -> __pipeRead);
    close(this -> __pipeWrite);
    close(this -> pipeRead);
    close(this -> pipeWrite);
    if (this -> clock_inevent != NULL) {
        close(this -> clock_inevent->data.clock_fd);
        delete this -> clock_inevent;
    }
    this->threadFd.join();
}

void Room::initTimer() {
    PPRINTF(this->logger, GREEN, "Initializing clock");
    if (this -> clock_inevent != NULL) return;
    struct itimerspec ts = {
        .it_interval = {
            .tv_sec = 1,
            .tv_nsec = 0,
        },
        .it_value = {
            .tv_sec = 1,
            .tv_nsec = 0,
        },
    };
    this -> clock_inevent = new InEvent;
    // TODO: handle errors here
    this->clock_inevent->data.clock_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    timerfd_settime(this->clock_inevent->data.clock_fd, 0, &ts, NULL);
    this->clock_inevent->src = RoomSource::CLOCK;
    struct epoll_event ev = {
        .events = EPOLLIN,
        .data = {.ptr = this->clock_inevent }
    };
    epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this->clock_inevent->data.clock_fd, &ev);
}

void Room::roomLoop() {
    struct epoll_event incomming;
    char buf[256];
    struct InEvent *ievent;
    Client *client;
    while (1) {
        if (stop) {
            PPRINTF(this->logger, BLUE, "Terminating room");
            break;
        }
        epoll_wait(this->epollFd, &incomming, 1, -1);

        ievent = ((InEvent *)incomming.data.ptr);
        if (ievent->src == RoomSource::CLOCK) {
            read(ievent->data.clock_fd, buf, 8);
            if (this->game->seconds_left == 0) {
                this->game->word = Words::get_word();
                PPRINTF(this->logger, YELLOW, "The new word is %s", this->game->word.c_str());
                this->game->seconds_left = this->roundTime;
                this->game->round_num++;

                for (auto i: this -> players) {
                    dprintf(i->data.client->socketDesc, "start %d\n", this->game->round_num);
                }

                if (this->describer == this->nPlayers - 1) this -> describer = 0;
                else this -> describer++;

                sprintf(buf, "describe %s\n", this->game->word.c_str());
                write(this->players[describer]->data.client->socketDesc, buf, strlen(buf));
            } else {
                PPRINTF(this->logger, GREEN, "Tik Tok %d seconds left", this->game->seconds_left);
                this->game->seconds_left--;
            }
            sprintf(buf, "clock %d\n", this->game->seconds_left);
            int len = strlen(buf);
            for (int i = 0; i < this -> players.size(); i++) {
                write(this->players[i]->data.client->socketDesc, buf, len);
            }
        } else {
            client = ievent->data.client;
            if (incomming.events & EPOLLRDHUP) {
                if (ievent == this->players[describer]) {
                    this->game->seconds_left = 0;
                }
                PPRINTF(this->logger, YELLOW,
                    "Client %s closed the connection", client->username.c_str());
                dprintf(this->__pipeWrite, "remove %p", client);
                this->unassign(ievent);
            } else if (incomming.events & EPOLLERR) {
                PPRINTF(this->logger, YELLOW, "EROR");
            } else {
                client->msgbuf->append(client->socketDesc);
                Command *c;
                while (1) {
                    /* if user leaves the room and then immediately runs another command in the lobby
                     * the second command might be read in the room
                     * to avoid the command being executed in the wrong context, we must check
                     * if the user is still in the room
                     */
                    if (client -> assignedRoom == NULL) break;
                    c = client->msgbuf->getCommand();
                    if (c == NULL) break;
                    this -> runCommand(c, ievent);
                    delete c;
                }
            }
        }
    }
}

void Room::runCommand(Command *c, InEvent *ie) {
    char *cmd = c->getCommand();
    Client *client = ie->data.client;
    char buf[256];
    if (!strcmp(cmd, "users")) {
        for (auto i: this->players) {
            int len = sprintf(buf, "%s %d\n", i->data.client->username.c_str(), i->data.client->score);
            write(client->socketDesc, buf, len);
        }
        write(client->socketDesc, "\n", 1);
    } else if (!strcmp(cmd, "msg")) {
        char *msg = c->getArgs();
        if (this->describer >= 0 && this->players[this->describer]->data.client != client && !strcmp(this->game->word.c_str(), msg)) {
            PPRINTF(this->logger, YELLOW, "User %s guessed the word", client->username.c_str());
            this->game->seconds_left = 0;
            sprintf(buf, "Win %s\n", client->username.c_str());
            client->score++;
            int len = strlen(buf);
            for (auto i: this -> players) {
                write(i->data.client->socketDesc, buf, len);
            }
        } else {
            for (auto i: this -> players) {
                dprintf(i->data.client->socketDesc,
                    "%d %s %s", i == this->players[this->describer], client->username.c_str(), msg);
            }
        }
    } else if (!strcmp(cmd, "leave")) {
        dprintf(this->__pipeWrite, "back %p", client);
        this->unassign(ie);
    } else {
        PPRINTF(this->logger, GREEN, "Unrecognized command");
    }
}

int Room::assign(Client *client) {
    if (this -> nPlayers == this -> maxPlayers) {
        PPRINTF(this -> logger, YELLOW, "User %s attempted to join a full room", client->username.c_str());
        return -1;
    }
    struct InEvent *ie = new InEvent;
    ie->data = {.client = client };
    ie->src = RoomSource::PLAYER;

    struct epoll_event ev = {
        .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
        .data= {.ptr = ie}
    };
    this -> players.push_back(ie);
    client->msgbuf->flush();
    client->score = 0;
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, client->socketDesc, &ev) == 0) {
        this -> nPlayers++;
        client -> assignedRoom = this;
        PPRINTF(this->logger, YELLOW, "Client %s assigned to room %s", client->username.c_str(), this->name.c_str());
        if (this -> nPlayers >= 3) {
            this->initTimer();
            this->initGame();
        }
        /* TODO: there can be some commands still queued in the client message buffer
         * We shouldn't run them here not to delay the assignment feedback
         * e.g. if we ran these commands here, the feedback from those commands would be
         * sent before assign feedback itself and it is not expected behavior.
         * This bug is not critical as queued commands will be executed after the next
         * input from the client */
        return 0;
    }
    return -1;
}

void Room::unassign(InEvent *ie) {
    epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, ie->data.client->socketDesc, NULL);
    this->nPlayers--;
    for (int i = 0; i < this->players.size(); i++) {
        if (this->players[i] == ie) {
            this -> players.erase(this->players.begin() + i);
            if (i < describer) describer--;
        }
    }
    ie->data.client->score=0;
    ie->data.client->assignedRoom=NULL;
    delete ie;

    if (this->nPlayers == 0) {
        write(this->__pipeWrite, "kill", 5);
        stop = true;
    }
}

int Room::getMaxPlayers() { return this -> maxPlayers; }
int Room::getNRounds() { return this -> nRounds; }
int Room::getRoundTime() { return this -> roundTime; }
int Room::getNPlayers() { return this -> nPlayers; }
