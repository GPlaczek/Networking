#include "client.hpp"

#include <arpa/inet.h>
#include <string>

Client::~Client() {
    delete this -> msgbuf;
}

Client::Client(
    int socketDesc,
    Room *assignedRoom,
    int msgbufLen
) {
    this->socketDesc=socketDesc;
    this->assignedRoom = assignedRoom;
    this->msgbuf = new MessageBuf(msgbufLen);
}
