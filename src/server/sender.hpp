#pragma once

#include "room.hpp"
#include "client.hpp"

enum Source {
    ROOM=1,
    CLIENT=2,
    SERVER=3
};

struct Sender {
    union Src {
        Room *room;
        Client *client;
        int fd;
    } data;
    enum Source src;
};

