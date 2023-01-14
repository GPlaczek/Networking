#include "client.hpp"

#include <arpa/inet.h>
#include <string>

Client::~Client() {
    delete this->address;
}

Client::Client(
    sockaddr_in *address,
    int socketDesc,
    Room *assignedRoom,
    MessageBuf msgbuf
) {
    this->address = address;
    this->socketDesc=socketDesc;
    this->assignedRoom = assignedRoom;
    this->msgbuf = msgbuf;
}
