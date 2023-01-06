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
// This functions returns the command name
// ptr - pointer to the rest of the message
char *get_command_name(char *buf, char **ptr) {
    char *command;
    *ptr = strchrnul(buf, ' ');
    int len = *ptr - buf;
    command = new char[len+1];
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
        close(user -> socketDesc);
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
            close(this -> socketFd);
            exit(1);
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
        // loop for accepting new connections
        struct sockaddr *addr;
        socklen_t addrlen;
        while(1) {
            addr = new sockaddr;
            // TODO: register new user, ask him for username and add him to some array or vector
            PPRINTF(this->logger, RED, "Waiting for new connections");
            int newFd = accept(this -> socketFd, addr, &addrlen);
            if (newFd == -1) {
                perror("Too many open files");
                exit(1);
            }
            PPRINTF(this->logger, RED, "Accepted a new connection");

            Client *client = new Client;
            client -> socketDesc = newFd;
            client -> address = (sockaddr_in*)addr;

            PPRINTF(this->logger, RED, "Adding new connection to epoll");
            // TODO: this leaks when user disconnects while in a room
            struct Sender *s1 = new Sender;
            s1->data = { .client = client };
            s1->src = Source::CLIENT;

            struct epoll_event ev = {
                .events = EPOLLIN | EPOLLRDHUP,
                .data= {.ptr = s1}
            };

            epoll_ctl(this -> epollFd, EPOLL_CTL_ADD, newFd, &ev);

            PPRINTF(this->logger, RED, "Adding new client to clients vector");
            this->clients.push_back(client);
        }
    }

    void localLoop() {
        struct epoll_event incomming;
        char buf[256];
        std::string message;
        char *ptr, *command;
        Sender *sender; 
        while(1) {
            memset(buf, 0, 256);

            epoll_wait(this->epollFd, &incomming, 1, -1);

            // cast user data to Sender instance pointer
            sender = ((Sender *)incomming.data.ptr);

            // check who sent the message
            if (sender->src == Source::CLIENT) {
                Client *client = sender->data.client;

                if (incomming.events & EPOLLRDHUP) {
                    PPRINTF(this->logger, BLUE, "Client %s closed the connection", client->username.c_str());
                    this->removeUser(client);
                    delete sender;
                } else if (read(client->socketDesc, buf, 256) > 0) {
                    if (client->username.empty()) {
                        // If the user didn't give a name, the first message he sends is his name
                        PPRINTF(this->logger, BLUE, "User %d registered as %s", client->socketDesc, buf);
                        client->username = buf;
                        client->assignedRoom = -1;
                        continue;
                    }
                    command = get_command_name(buf, &ptr);

                    if (!strcmp(command, "create")) {
                        // create a new room
                        PPRINTF(this->logger, RED, "User %s creates a new room", client->username.c_str());

                        Room *room = new Room(3, 2, 2, client);

                        this->rooms.push_back(room);
                        std::thread roomTh(&Room::roomLoop, room);
                        room->threadFd = std::move(roomTh);
                        room->assign(client);
                        epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
                        // for some reason declaring Sender as not a pointer breaks it
                        // TODO: free this pointer after removing a room
                        struct Sender *sr = new Sender;
                        sr->data = {.room=room};
                        sr->src = Source::ROOM;

                        // epoll_event for reading from room pipe
                        struct epoll_event ev = {
                            .events = EPOLLIN,
                            .data= {.ptr = sr}
                        };

                        epoll_ctl(this->epollFd, EPOLL_CTL_ADD, room->pipeRead, &ev);
                        message = "Room " + std::to_string(client->assignedRoom) + " successfully created\n";                       
                        write(client->socketDesc, message.c_str(), message.length() + 1);
                    } else if (!strcmp(command, "join")) {
                        // join a room
                        int room;
                        sscanf(ptr, "%d", &room);
                        PPRINTF(this->logger, BLUE, "User %s wants to join %d", client->username.c_str(), room);
                        if (client->assignedRoom != -1) {
                            PPRINTF(this->logger, BLUE, "User %s is already assigned to a room %i", client->username.c_str(), client->assignedRoom);
                            message = "You are already assigned to the room " + std::to_string(client->assignedRoom) + "\n";
                            write(client->socketDesc, message.c_str(), message.length() + 1);
                        } else {
                            try {
                                this->rooms.at(room)->assign(client);
                                epoll_ctl(this -> epollFd, EPOLL_CTL_DEL, client->socketDesc, NULL);
                                message = "Successfully joined the room " + std::to_string(room) + "\n";
                                write(client->socketDesc, message.c_str(), message.length() + 1);
                            } catch (...) {
                                write(client->socketDesc, "No such room\n", 14);
                            }
                        }
                    } else {
                        PPRINTF(this->logger, BLUE, "User %s sent: %s", client->username.c_str(), buf);
                    }
                    delete [] command;
                } else {
                    fprintf(stderr, "Possible missing epoll event");
                    exit(1);
                }
                write(client->socketDesc, "leave me alone\n", 16);
            } else if (sender->src == Source::ROOM) {
                // for messages sent from room pipes
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
