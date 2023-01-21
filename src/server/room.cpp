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
    this->game->seconds_left = this ->roundTime;
    this->game->word = Words::get_word();
    this->describer = 0;
    PPRINTF(this->logger, YELLOW, "The word is %s", this->game->word.c_str());
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

    int pp[2];
    pipe2(pp, O_DIRECT);
    this -> __pipeRead = pp[0];
    this -> pipeWrite = pp[1];
    pipe2(pp, O_DIRECT);
    this -> pipeRead = pp[0];
    this -> __pipeWrite = pp[1];
}

Room::~Room() {
    close(this -> epollFd);
    close(this -> __pipeRead);
    close(this -> __pipeWrite);
    close(this -> pipeRead);
    close(this -> pipeWrite);
    if (this -> clock_inevent != NULL) {
        close(this -> clock_inevent->data.clock_fd);
        delete this -> clock_inevent;
    }
    threadFd.join();
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
        epoll_wait(this->epollFd, &incomming, 1, -1);

        ievent = ((InEvent *)incomming.data.ptr);
        if (ievent->src == RoomSource::CLOCK) {
            read(ievent->data.clock_fd, buf, 8);
            if (this->game->seconds_left == 0) {
                this->game->word = Words::get_word();
                PPRINTF(this->logger, YELLOW, "The new word is %s", this->game->word.c_str());
                this->game->seconds_left = this->roundTime;
                if (this->describer == this->nPlayers - 1) this -> describer = 0;
                else this -> describer++;
            } else {
                PPRINTF(this->logger, GREEN, "Tik Tok %d seconds left", this->game->seconds_left);
                this->game->seconds_left--;
            }
            sprintf(buf, "%d\n", this->game->seconds_left);
            for (int i = 0; i < this -> players.size(); i++) {
                write(this->players[i]->data.client->socketDesc, buf, strlen(buf));
            }
        } else {
            client = ievent->data.client;
            if (incomming.events & EPOLLRDHUP) {
                PPRINTF(this->logger, YELLOW, "Client %s closed the connection", client->username.c_str());
                this->unassign(ievent);
                dprintf(this->__pipeWrite, "remove %p", client);
            } else if (incomming.events & EPOLLERR) {
                PPRINTF(this->logger, YELLOW, "EROR");
            } else {
                read(client->socketDesc, buf, 256);
                if (!strcmp(this->game->word.c_str(), buf)) {
                    PPRINTF(this->logger, YELLOW, "User %s guessed the word", client->username.c_str());
                    this->game->round_num++;
                    this->game->seconds_left = 0;
                }
                PPRINTF(this->logger, YELLOW, "User %s send %s to the second thread",
                    client->username.c_str(), buf);
            }
        }
    }
}

void Room::assign(Client *client) {
    struct InEvent *ie = new InEvent;
    ie->data = {.client = client };
    ie->src = RoomSource::PLAYER;

    struct epoll_event ev = {
        .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
        .data= {.ptr = ie}
    };
    this -> players.push_back(ie);
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, client->socketDesc, &ev) == 0) {
        this -> nPlayers++;
        client -> assignedRoom = this;
        PPRINTF(this->logger, YELLOW, "Client %s assigned to room %s", client->username.c_str(), this->name.c_str());
        if (this -> nPlayers >= 3) {
            this->initTimer();
            this->initGame();
        }
    }
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
    ie->data.client -> assignedRoom = NULL;
    delete ie;
}

int Room::getMaxPlayers() { return this -> maxPlayers; }
int Room::getNRounds() { return this -> nRounds; }
int Room::getRoundTime() { return this -> roundTime; }
int Room::getNPlayers() { return this -> nPlayers; }
