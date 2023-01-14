#pragma once

#include <arpa/inet.h>
#include <string>

#include "msgbuf.hpp"

class Client {
public:
    std::string username;
    sockaddr_in *address;
    int socketDesc;
    int assignedRoom;
    MessageBuf msgbuf;

    Client(
        sockaddr_in *address,
        int socketDesc,
        int assignedRoom,
        MessageBuf msgbuf
    );
    ~Client();
};
