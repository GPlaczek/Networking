#pragma once

#include "client.hpp"
#include <thread>

class Room {
    int epollFd;
    int maxPlayers, nRounds, roundTime;
    int nPlayers;
    Client *describer;
public:
    std::thread threadFd;
    Room(int maxPlayers, int nRounds, int roundTime, Client *describer);
    ~Room();
    void roomLoop();
    void assign(Client *client);
    void unassign(Client *client);
};
