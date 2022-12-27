#pragma once

#include "client.hpp"
#include "log.hpp"

#include <thread>

class Room {
    int __pipeRead, __pipeWrite;
    int epollFd;
    int maxPlayers, nRounds, roundTime;
    int nPlayers;
    Client *describer;
    Log logger;
public:
    int pipeRead, pipeWrite;
    std::thread threadFd;
    Room(int maxPlayers, int nRounds, int roundTime, Client *describer);
    ~Room();
    void roomLoop();
    void assign(Client *client);
    void unassign(Client *client);
};
