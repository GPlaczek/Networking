#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>

#include <sys/socket.h>
#include <sys/epoll.h>

#include <arpa/inet.h>

#include "client.hpp"
#include "room.hpp"
#include "log.hpp"
#include "sender.hpp"

#define OPTSTRING "hi:p:r:c:q:"
#define N_ROOMS_DEFAULT 16
#define N_CLIENTS_DEFAULT 64
#define QUEUE_LEN_DEFAULT 32

// Remember to free the returned char pointer afterwards
char *get_command_name(char *buf, char **ptr) {
    char *command;
    *ptr = strchrnul(buf, ' ');
    int len = *ptr - buf;
    command = new char[len];
    memcpy(command, buf, len);
    command[len] = 0;
    return command;
}

void print_help(const char *name) {
    printf("server - Taboo game server\n"
        "Usage: %s [options]\n\nOptions:\n"
        " -i <ip address>\n\tip address of the server\n"
        " -p <port number>\n\tport for the listenning socket\n"
        " -r <integer>\n\tmax number of rooms (default is %d)\n"
        " -c <integer>\n\tmax number of clients (default is %d)\n"
        " -q <integer>\n\tsocket queue length (default is %d)\n",
        name, N_ROOMS_DEFAULT, N_CLIENTS_DEFAULT, QUEUE_LEN_DEFAULT);
}

class Server {
    size_t nRooms;
    size_t nClients;

    sockaddr_in *address;

    int socketFd;
    Log logger;

    int epollFd;
    // TODO: protect this vector with a mutex
    std::vector <Client*> clients;

    // TODO: if clients was an unordered map, this function could be much faster
    void removeUser(Client *user) {
        PPRINTF(this->logger, GREEN, "Removing user %s from the server", user->username.c_str());
        this -> clients.erase(std::remove(this->clients.begin(), this->clients.end(), user), this->clients.end());
        epoll_ctl(this->epollFd, EPOLL_CTL_DEL, user->socketDesc, NULL);
        delete user;
    }

public:
    std::vector <Room*> rooms;

    Server(sockaddr_in *addr, size_t nRooms, size_t nClients, size_t queueLen) {
        this -> nRooms = nRooms;
        this -> nClients = nClients;
        this -> address = addr;

        this -> socketFd = socket(AF_INET, SOCK_STREAM, 0);
        this -> epollFd = epoll_create(nClients);

        int one = 1;
        setsockopt(this -> socketFd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

        if (bind(this -> socketFd, (sockaddr*)(this -> address), sizeof(*(this -> address)))) {
            perror("Could not bind to the given address");
        }
        listen(this -> socketFd, queueLen);
    }

    ~Server() {
        close(this -> socketFd);
    }

    void assignToRoom(Client *client, Room *room) {
        epoll_ctl(this->epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
        room->assign(client);
    }

    void listenLoop() {
        struct sockaddr *addr;
        socklen_t addrlen;
        while(1) {
            addr = new sockaddr;
            // TODO: register new user, ask him for username and add him to some array or vector
            PPRINTF(this->logger, RED, "Waiting for new connections");
            int newFd = accept(this -> socketFd, addr, &addrlen);
            PPRINTF(this->logger, RED, "Accepted a new connection");

            Client *client = new Client;
            client -> socketDesc = newFd;
            client -> address = (sockaddr_in*)addr;

            PPRINTF(this->logger, RED, "Adding new connection to epoll");
            struct Sender *s1 = new Sender;
            s1->data = { .client = client };
            s1->src = Source::CLIENT;
            struct epoll_event ev = {
                .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
                .data= {.ptr = s1}
            };
            int stat = epoll_ctl(this -> epollFd, EPOLL_CTL_ADD, newFd, &ev);

            PPRINTF(this->logger, RED, "Adding new client to clients vector");
            this->clients.push_back(client);
        }
    }

    void localLoop() {
        struct epoll_event incomming;
        char buf[256];
        char *ptr, *command;
        Sender *sender; 
        while(1) {
            memset(buf, 0, 256);

            // DEBUG_PRINT(BLUE, "Polling on existing connections");
            epoll_wait(this->epollFd, &incomming, 1, -1);

            sender = ((Sender *)incomming.data.ptr);
            if (sender->src == Source::CLIENT) {
                Client *client = sender->data.client;
                if (incomming.events & EPOLLRDHUP) {
                    PPRINTF(this->logger, BLUE, "Client %s closed the connection", client->username.c_str());
                    this->removeUser(client);
                } else if (read(client->socketDesc, buf, 256) > 0) {
                    if (client->username.empty()) {
                        PPRINTF(this->logger, BLUE, "User %d registered as %s", client->socketDesc, buf);
                        client->username = buf;
                        continue;
                    }
                    command = get_command_name(buf, &ptr);
                    if (!strcmp(command, "create")) {
                        PPRINTF(this->logger, RED, "User %s creates a new room", client->username.c_str());
                        Room *room = new Room(3, 2, 2, client);
                        this->rooms.push_back(room);
                        std::thread roomTh(&Room::roomLoop, room);
                        room->threadFd = std::move(roomTh);
                        // for some reason declaring Sender as not a pointer breaks it
                        struct Sender *sr = new Sender;
                        sr->data = {.room=room};
                        sr->src = Source::ROOM;
                        struct epoll_event ev = {
                            .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
                            .data= {.ptr = sr}
                        };
                        epoll_ctl(this->epollFd, EPOLL_CTL_ADD, room->pipeRead, &ev);
                    } else if (!strcmp(command, "join")) {
                        int room;
                        sscanf(ptr, "%d", &room);
                        PPRINTF(this->logger, BLUE, "User %s wants to join %d", client->username.c_str(), room);
                        this->rooms.at(room)->assign(client);
                        epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
                    } else {
                        PPRINTF(this->logger, BLUE, "User %s sent: %s", client->username.c_str(), buf);
                    }
                    delete [] command;
                } else {
                    fprintf(stderr, "Possible missing epoll event");
                    exit(1);
                }
                write(client->socketDesc, "leave me alone", 14);
            } else if (sender->src == Source::ROOM) {
                Room *room = sender->data.room;
                if (read(room->pipeRead, buf, 256) > 0) {
                    command = get_command_name(buf, &ptr);
                    PPRINTF(this->logger, GREEN, "Room sent command %s", command);
                    if (!strcmp(command, "remove")) {
                        Client *client;
                        sscanf(ptr, "%p", &client);
                        this->removeUser(client);
                    }
                    PPRINTF(this->logger, GREEN, "Room sent %s", buf);
                }
            }
        }
    }
};

int main(int argc, char *argv[]) {
    in_addr_t ipaddr;
    int opt, port;
    size_t rooms = N_ROOMS_DEFAULT,
        clients = N_CLIENTS_DEFAULT,
        queueLen = QUEUE_LEN_DEFAULT;

    bool pPort = 0,
        pAddr = 0;
    while((opt = getopt(argc, argv, OPTSTRING)) != -1) {
        switch (opt) {
            case 'i':
                ipaddr = inet_addr(optarg);
                if (ipaddr == -1) {
                    fprintf(stderr, "Invalid ip address\n");
                    exit(1);
                }
                pAddr = 1;
                break;
            case 'p':
                if (sscanf(optarg, "%d", &port) == 0) {
                    fprintf(stderr, "Invalid port\n");
                    exit(1);
                }
                // port = htons(port);
                pPort = 1;
                break;
            case 'r':
                if (sscanf(optarg, "%zu", &rooms) == 0) {
                    fprintf(stderr, "Invalid number of rooms\n");
                    exit(1);
                }
                break;
            case 'c':
                if (sscanf(optarg, "%zu", &clients) == 0) {
                    fprintf(stderr, "Invalid number of clients\n");
                    exit(1);
                }
                break;
            case 'q':
                if (sscanf(optarg, "%zu", &queueLen) == 0) {
                    fprintf(stderr, "Invalid queue length\n");
                    exit(1);
                }
                break;
            case 'h':
                print_help(argv[0]);
                exit(0);
        }
    }

    if (!pPort) {
        fprintf(stderr, "No port specified\n");
        print_help(argv[0]);
        return 1;
    }
    if (!pAddr) {
        fprintf(stderr, "No ip address specified\n");
        print_help(argv[0]);
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ipaddr;

    Server s(&addr, rooms, clients, queueLen);
    std::thread mainLoop(&Server::listenLoop, std::ref(s));
    s.localLoop();

    mainLoop.join();

    for(int i = 0; i < s.rooms.size(); i++) {
        delete s.rooms[i];
    }

    return 0;
}
