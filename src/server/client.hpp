#pragma once

#include <arpa/inet.h>
#include <string>

#include "msgbuf.hpp"

class Room;

class Client {
public:
    int score;
    std::string username;
    int socketDesc;
    Room *assignedRoom;
    MessageBuf *msgbuf;

    Client(
        int socketDesc,
        Room *assignedRoom,
        int msgbufLen
    );
    ~Client();
};
