#include "client.hpp"
#include "room.hpp"
#include "log.hpp"

#include <thread>
#include <sys/epoll.h>
#include <unistd.h>

Room::Room(int maxPlayers, int nRounds, int roundTime, Client* describer) {
    this -> maxPlayers = maxPlayers;
    this -> nRounds = nRounds;
    this -> roundTime = roundTime;
    this -> describer = describer;
    this -> nPlayers = 0;
    this -> epollFd = epoll_create(maxPlayers);
}

Room::~Room() {
    close(this -> epollFd);
    threadFd.join();
}

void Room::roomLoop() {
    struct epoll_event incomming;
    char buf[256];
    Client *client;
    while (1) {
        epoll_wait(this->epollFd, &incomming, 1, -1);

        client = ((Client *)incomming.data.ptr);

        if (incomming.events & EPOLLRDHUP) {
            PPRINTF(this->logger, YELLOW, "Client %s closed the connection", client->username.c_str());
            this->unassign(client);
        } else if (incomming.events & EPOLLERR) {
            PPRINTF(this->logger, YELLOW, "EROR");
        } else {
            read(client->socketDesc, buf, 256);
            PPRINTF(this->logger, YELLOW, "User %s send %s to the second thread", client->username.c_str(), buf);
        }
    }
}

void Room::assign(Client *client) {
    struct epoll_event ev = {
        .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
        .data= {.ptr = client}
    };
    if (epoll_ctl(this->epollFd, EPOLL_CTL_ADD, client->socketDesc, &ev) == 0) {
        this -> nPlayers++;
        PPRINTF(this->logger, YELLOW, "Client %s assigned", client->username.c_str());
    }
}

void Room::unassign(Client *client) {
    epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
    this->nPlayers--;
}
