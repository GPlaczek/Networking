#pragma once

#include <arpa/inet.h>
#include <string>

class Client {
public:
    std::string username;
    sockaddr_in *address;
    int socketDesc;
    ~Client();
};
