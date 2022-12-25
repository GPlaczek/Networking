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
    this -> epollFd = epoll_create(maxPlayers);
}

Room::~Room() {
    close(this -> epollFd);
    threadFd.join();
}

void Room::roomLoop() {
}
