#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "msgbuf.hpp"

MessageBuf::MessageBuf(){}

MessageBuf::MessageBuf(int size) {
    printf("Debuggin\n");
    this -> buf = new char[size];
    memset(buf, 0, size);
    this -> bufSize = size;
    this -> contentLen = 0;
    this -> bufPos = 0;
}

MessageBuf::~MessageBuf() {
    printf("Deletin\n");
    delete [] this -> buf;
}

void MessageBuf::append(int desc) {
    int len = read(desc, this->buf + this->contentLen, this->bufSize - this->contentLen - 1);
    this -> contentLen += len;
}

Command *MessageBuf::getCommand() {
    char *inputEnd = (char*)memchr(this->buf+this->bufPos, '\n', this->contentLen);
    if (inputEnd == NULL) { return NULL; }
    inputEnd[0] = 0;
    int __bufPos = this->bufPos;
    this->bufPos = inputEnd+1-this->buf;
    return new Command(this->buf+__bufPos);
}

void MessageBuf::shift() {
    memmove(this->buf, this->buf+this->bufPos, this->contentLen);
    this->contentLen -= this->bufPos;
    this->bufPos = 0;
}

void MessageBuf::flush() {
    this->contentLen = 0;
    this->bufPos = 0;
}

Command::Command() {
    this->command = NULL;
    this->args = NULL;
}

Command::Command(char *command){
    this->command = command;
    char *commandEnd = (char*)strchrnul(this->command, ' ');
    if (commandEnd == NULL) {
        this->args = NULL;
    } else {
        commandEnd[0] = 0;
        this->args = commandEnd+1;
    }
}

char *Command::getCommand() { return this->command; }

char *Command::getArgs() { return this -> args; }
