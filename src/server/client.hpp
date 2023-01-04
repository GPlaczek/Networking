#pragma once

#include <arpa/inet.h>
#include <string>

class Client {
public:
    std::string username;
    sockaddr_in *address;
    int socketDesc;
    // TODO: change isAssigned to room identifier
    int isAssigned;
    ~Client();
};
