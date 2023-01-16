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
#include <netinet/tcp.h>

#include <arpa/inet.h>

#include "client.hpp"
#include "room.hpp"
#include "log.hpp"
#include "sender.hpp"
#include "msgbuf.hpp"

#define OPTSTRING "hi:p:r:c:q:"
#define N_ROOMS_DEFAULT 16
#define N_CLIENTS_DEFAULT 64
#define QUEUE_LEN_DEFAULT 32
#define DEFAULT_MSGBUF_LEN 2048

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

        int sockopt = 1;
        setsockopt(this -> socketFd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt));
        setsockopt(this -> socketFd, SOL_SOCKET, SO_KEEPALIVE, &sockopt, sizeof(sockopt));
        setsockopt(this -> socketFd, SOL_TCP, TCP_KEEPINTVL, &sockopt, sizeof(sockopt));
        sockopt = 5;
        setsockopt(this -> socketFd, SOL_TCP, TCP_KEEPCNT, &sockopt, sizeof(sockopt));
        sockopt = 10;
        setsockopt(this -> socketFd, SOL_TCP, TCP_KEEPIDLE, &sockopt, sizeof(sockopt));

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

    void runCommand(Command *command, Client *client) {
        char *cmd = command->getCommand();
        char *args = command->getArgs();
        std::string message;
        static char buf[256];
        if (!strcmp(cmd, "create")) {
            // create a new room
            if (args == NULL) {
                PPRINTF(this->logger, RED, "User %s did not entered room name", client->username.c_str());
                message = "Please enter room name after 'create' command\n";
                write(client->socketDesc, message.c_str(), message.length() + 1);
            } else {
                //check if room name is unique
                bool unique = true;
                for (int i = 0; i < this->rooms.size(); i++) {
                    if (this->rooms[i]->name == args) {
                        unique = false;
                        PPRINTF(this->logger, RED, "Room name %s is not unique", args);
                        message = "Please enter different room name. This one is not unique\n";
                        write(client->socketDesc, message.c_str(), message.length() + 1);
                        break;
                    }
                }
                if (unique) {
                    PPRINTF(this->logger, RED, "User %s creates a new room", client->username.c_str());
                    Room *room = new Room(3, 2, 2, client);

                    this->rooms.push_back(room);
                    std::thread roomTh(&Room::roomLoop, room);
                    room->threadFd = std::move(roomTh);
                    room->assign(client);
                    room->name = args;
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
                    message = "Room " + room->name + " successfully created\n";                       
                    write(client->socketDesc, message.c_str(), message.length() + 1);
                }
            }
        } else if (!strcmp(cmd, "join")) {
            // join a room
            int room;
            sscanf(args, "%d", &room);
            PPRINTF(this->logger, BLUE, "User %s wants to join %d", client->username.c_str(), room);
            if (client->assignedRoom != NULL) {
                PPRINTF(this->logger, BLUE, "User %s is already assigned to a room %i", client->username.c_str(), client->assignedRoom);
                message = "You are already assigned to the room " + client->assignedRoom->name + "\n";
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
        } else if (!strcmp(cmd, "msg")) {
            PPRINTF(this->logger, BLUE, "User %s sent: %s", client->username.c_str(), args);
        } else if (!strcmp(cmd, "users")) {
            for (auto i: this->clients) {
                int len;
                if (i->assignedRoom == NULL) {
                    len = sprintf(buf, "%s\n", i->username.c_str());
                } else {
                    len = sprintf(buf, "%s %s\n",
                        i->username.c_str(), i->assignedRoom->name.c_str());
                }
                write(client->socketDesc, buf, len);
            }
            write(client->socketDesc, "\n", 1);
        } else if (!strcmp(cmd, "rooms")) {
            for (auto i: this->rooms) {
                int len;
                len = sprintf(buf, "%s %d %d %d\n",
                    i->name.c_str(),
                    i->getMaxPlayers(),
                    i->getNRounds(),
                    i->getRoundTime());
                write(client->socketDesc, buf, len);
            }
            write(client->socketDesc, "\n", 1);
        } else {
            PPRINTF(this->logger, BLUE, "Unrecognized command: %s", cmd);
        }
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

            Client *client = new Client((sockaddr_in*)addr, newFd, NULL, MessageBuf(DEFAULT_MSGBUF_LEN));

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
                } else if (incomming.events & EPOLLIN) {
                    client->msgbuf.append(client->socketDesc);
                    Command *c;
                    while (1) {
                        c = client->msgbuf.getCommand();
                        if (c == NULL) { break; };
                        if (client->username.empty()) {
                            // If the user didn't give a name, the first message he sends is his name
                            client->username = c->getCommand();
                            PPRINTF(this->logger, BLUE, "User %d registered as %s", client->socketDesc, client->username.c_str());
                            client->assignedRoom = NULL;
                            continue;
                        }
                        this->runCommand(c, client);
                        delete c;
                    }
                } else {
                    fprintf(stderr, "Possible missing epoll event");
                    exit(1);
                }
            } else if (sender->src == Source::ROOM) {
                memset(buf, 0, 256);
                // for messages sent from room pipes
                Room *room = sender->data.room;
                if (read(room->pipeRead, buf, 256) > 0) {
                    Command command = Command(buf);
                    char *cmd = command.getCommand();
                    char *args = command.getArgs();
                    PPRINTF(this->logger, GREEN, "Room sent command %s", command);
                    if (!strcmp(cmd, "remove")) {
                        Client *client;
                        sscanf(args, "%p", &client);
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
