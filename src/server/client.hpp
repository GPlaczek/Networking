#pragma once

#include <arpa/inet.h>
#include <string>

#include "msgbuf.hpp"

class Room;

class Client {
public:
    int score;
    std::string username;
    sockaddr_in *address;
    int socketDesc;
    Room *assignedRoom;
    MessageBuf msgbuf;

    Client(
        sockaddr_in *address,
        int socketDesc,
        Room *assignedRoom,
        MessageBuf msgbuf
    );
    ~Client();
};
