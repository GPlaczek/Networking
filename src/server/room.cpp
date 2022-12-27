#include "client.hpp"
#include "room.hpp"

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
            printf("Client %s closed the connection\n", client->username.c_str());
            this->unassign(client);
        } else if (read(client->socketDesc, buf, 256) > 0) {
            printf("User %s send %s to the second thread\n", client->username.c_str(), buf);
        }
    }
}

void Room::assign(Client *client) {
    static struct epoll_event ev = {
        .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
        .data= {.ptr = client}
    };
    epoll_ctl(this->epollFd, EPOLL_CTL_ADD, client->socketDesc, &ev);
    this -> nPlayers++;
}

void Room::unassign(Client *client) {
    epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
    this->nPlayers--;
}
