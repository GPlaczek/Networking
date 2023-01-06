#pragma once

#include "room.hpp"
#include "client.hpp"

enum Source {
    ROOM=1,
    CLIENT=2,
    SERVER=3
};

class Sender {
    union Src {
        int fd;
        Room *room;
        Client *client;
    } data;
    enum Source src;
public:
    Sender();
};

