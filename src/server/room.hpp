#pragma once

#include "client.hpp"
#include "log.hpp"

#include <thread>

struct game {
    int seconds_left;
    int round_num;
    std::string word;
};

class Room {
    struct game *game;
    int timerfd, timer;
    int __pipeRead, __pipeWrite; // room pipe ends
    int epollFd;
    int maxPlayers, nRounds, roundTime;
    int nPlayers;
    Client *describer;
    Log logger;
    void initTimer();
    void initGame();
public:
    int pipeRead, pipeWrite; // server pipe ends
    std::thread threadFd;
    std::string name;
    Room(int maxPlayers, int nRounds, int roundTime, Client *describer);
    ~Room();
    void roomLoop();
    void assign(Client *client);
    void unassign(Client *client);

    int getMaxPlayers();
    int getNRounds();
    int getRoundTime();
    int getNPlayers();
};
