#include "client.hpp"

#include <arpa/inet.h>
#include <string>

Client::~Client() {
    delete this->address;
}

