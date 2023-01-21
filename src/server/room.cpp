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

enum Source {
    CLIENT=0,
    CLOCK=1,
};

struct InEvent {
    union Stc {
        Client *client;
        int clock_fd;
    } data;
    enum Source src;
};

void Room::initGame() {
    this->game = new struct game;
    this->game->round_num = 0;
    this->game->seconds_left = this ->roundTime;
    this->game->word = Words::get_word();
    PPRINTF(this->logger, YELLOW, "The word is %s", this->game->word.c_str());
}

Room::Room(int maxPlayers, int nRounds, int roundTime, Client* describer) {
    this -> maxPlayers = maxPlayers;
    this -> nRounds = nRounds;
    this -> roundTime = roundTime;
    this -> describer = describer;
    this -> nPlayers = 0;
    this -> epollFd = epoll_create(maxPlayers);
    this -> timerfd = -1;

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
    if (this -> timerfd != -1) close(this -> timerfd);
    threadFd.join();
}

void Room::initTimer() {
    if (this -> timerfd != -1) return;
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
    // TODO: handle errors here
    this -> timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    timerfd_settime(this->timerfd, 0, &ts, NULL);
    // TODO: if this doesn't leak, then my name is Barrack Obama
    struct InEvent *ie = new InEvent;
    ie->data = {.clock_fd = this->timerfd };
    ie->src = Source::CLOCK;
    struct epoll_event ev = {
        .events = EPOLLIN,
        .data = {.ptr = ie }
    };
    epoll_ctl(this->epollFd, EPOLL_CTL_ADD, this -> timerfd, &ev);
}

void Room::roomLoop() {
    struct epoll_event incomming;
    char buf[256];
    struct InEvent *ievent;
    Client *client;
    while (1) {
        epoll_wait(this->epollFd, &incomming, 1, -1);

        ievent = ((InEvent *)incomming.data.ptr);
        if (ievent->src == Source::CLOCK) {
            this -> timer--;
            read(this->timerfd, buf, 8);
            if (this->game->seconds_left == 0) {
                this->game->word = Words::get_word();
                PPRINTF(this->logger, YELLOW, "The new word is %s", this->game->word.c_str());
                this->game->seconds_left = this->roundTime;
            } else {
                PPRINTF(this->logger, GREEN, "Tik Tok %d seconds left", this->game->seconds_left);
                this->game->seconds_left--;
            }
        } else {
            client = ievent->data.client;
            if (incomming.events & EPOLLRDHUP) {
                PPRINTF(this->logger, YELLOW, "Client %s closed the connection", client->username.c_str());
                this->unassign(client);
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
    ie->src = Source::CLIENT;

    struct epoll_event ev = {
        .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
        .data= {.ptr = ie}
    };
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

void Room::unassign(Client *client) {
    epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
    this->nPlayers--;
    client -> assignedRoom = NULL;
}

int Room::getMaxPlayers() { return this -> maxPlayers; }
int Room::getNRounds() { return this -> nRounds; }
int Room::getRoundTime() { return this -> roundTime; }
int Room::getNPlayers() { return this -> nPlayers; }
