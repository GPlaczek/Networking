#pragma once

class Command {
    char *command;
    char *args;
public:
    Command();
    Command(char *cmd);
    char *getCommand();
    char *getArgs();
};

class MessageBuf {
    int bufSize;
    int contentLen;
    int bufPos;
    char *buf;
public:
    MessageBuf();
    MessageBuf(int size);
    ~MessageBuf();
    void append(int desc);
    Command *getCommand();
    void shift();
    void flush();
};
