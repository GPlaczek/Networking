#pragma once

#include "client.hpp"
#include "msgbuf.hpp"
#include "log.hpp"

#include <thread>
#include <vector>

struct game {
    int seconds_left;
    int round_num;
    std::string word;
};

enum RoomSource {
    PLAYER=0,
    CLOCK=1,
};

struct InEvent {
    union Stc {
        Client *client;
        int clock_fd;
    } data;
    enum RoomSource src;
};

class Room {
    struct game *game;
    struct InEvent *clock_inevent;
    std::vector<struct InEvent*> players;

    bool stop;
    int __pipeRead, __pipeWrite; // room pipe ends
    int epollFd;
    int maxPlayers, nRounds, roundTime;
    int nPlayers;
    int describer;
    Log logger;
    void initTimer();
    void initGame();
    void runCommand(Command *c, InEvent *ie);
public:
    int pipeRead, pipeWrite; // server pipe ends
    std::thread threadFd;
    std::string name;
    Room(int maxPlayers, int nRounds, int roundTime);
    ~Room();
    void roomLoop();
    void assign(Client *client);
    void unassign(InEvent *ie);

    int getMaxPlayers();
    int getNRounds();
    int getRoundTime();
    int getNPlayers();
};
