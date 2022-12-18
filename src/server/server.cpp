#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>

#include <sys/socket.h>
#include <sys/epoll.h>

#include <arpa/inet.h>

#define OPTSTRING "hi:p:r:c:q:"
#define N_ROOMS_DEFAULT 16
#define N_CLIENTS_DEFAULT 64
#define QUEUE_LEN_DEFAULT 32

#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34

// This will break if someone wants to use some other variable called __buffer in this macro
#define PPRINTF(COLOR, FORMAT, args...) ({\
    char __buffer[9];\
    get_time(__buffer);\
    printf("\033[%d;1m[%s]\033[0m \033[35;1m[%s]\033[0m " FORMAT "\n", COLOR, __buffer, __func__, ##args);\
})

#ifdef LOG
#define DEBUG_PRINT(COLOR, FORMAT, ...) PPRINTF(COLOR, FORMAT, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif

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

void get_time(char *buf) {
    // TODO: handle errors here
    time_t timer;\
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);
    strftime(buf, 9, "%H:%M:%S", tm_info);
}

class Client {
    public: 
        std::string username;
        sockaddr_in *address;
        int socketDesc;
        //assigned room?
};

class Room {
    int epollFd;
    int maxPlayers, nRounds, roundTime;
    Client* describer;
    // std::vector <Client*> clients;
    public:
        std::thread threadFd;

        Room(int maxPlayers, int nRounds, int roundTime, Client* describer) {
            this -> maxPlayers = maxPlayers;
            this -> nRounds = nRounds;
            this -> roundTime = roundTime;
            this -> describer = describer;
            this -> epollFd = epoll_create(maxPlayers);
        }

        ~Room() {
            close(this -> epollFd);
            threadFd.join();
        }

        void roomLoop() {
            // while (1)
            // {
            
            // }
        }
};

class Server {
    size_t nRooms;
    size_t nClients;

    sockaddr_in *address;

    int socketFd;

    int epollFd;
    // TODO: protect this vector with a mutex
    std::vector <Client*> clients;

    // TODO: if clients was an unordered map, this function could be much faster
    void removeUser(Client *user) {
        DEBUG_PRINT(GREEN, "Removing user %s from the server", user->username.c_str());
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

    void listenLoop() {
        struct sockaddr *addr;
        socklen_t addrlen;
        while(1) {
            addr = new sockaddr;
            // TODO: register new user, ask him for username and add him to some array or vector
            DEBUG_PRINT(RED, "Waiting for new connections");
            int newFd = accept(this -> socketFd, addr, &addrlen);
            DEBUG_PRINT(RED, "Accepted a new connection");

            Client *client = new Client;
            client -> socketDesc = newFd;
            client -> address = (sockaddr_in*)addr;

            DEBUG_PRINT(RED, "Adding new connection to epoll");
            struct epoll_event ev = {
                .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLRDHUP,
                .data= {.ptr = client}
            };
            epoll_ctl(this -> epollFd, EPOLL_CTL_ADD, newFd, &ev);

            DEBUG_PRINT(RED, "Adding new client to clients vector");
            this->clients.push_back(client);
        }
    }

    void localLoop() {
        struct epoll_event incomming;
        char buf[256];
        Client *client; 
        while(1) {
            memset(buf, 0, 256);

            // DEBUG_PRINT(BLUE, "Polling on existing connections");
            epoll_wait(this->epollFd, &incomming, 1, -1);

            client = ((Client *)incomming.data.ptr);

            if (incomming.events & EPOLLRDHUP) {
                PPRINTF(BLUE, "Client %s closed the connection", client->username.c_str());
                this->removeUser(client);
            } else if (read(client->socketDesc, buf, 256) > 0) {
                if (client->username.empty()) {
                    DEBUG_PRINT(BLUE, "User %d registered as %s", client->socketDesc, buf);
                    client->username = buf;
                } else if (!strcmp(buf, "create")) {
                    Room *room = new Room(3, 2, 2, client);
                    this->rooms.push_back(room);
                    std::thread roomTh(&Room::roomLoop, room);
                    room->threadFd = std::move(roomTh);
                } else {
                    PPRINTF(BLUE, "User %s sent: %s", client->username.c_str(), buf);
                }
            } else {
                fprintf(stderr, "Possible missing epoll event");
                exit(1);
            }
            write(client->socketDesc, "leave me alone", 14);
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

    DEBUG_PRINT(YELLOW, "Creating Server instance");
    Server s(&addr, rooms, clients, queueLen);
    DEBUG_PRINT(YELLOW, "Entering server listenner loop");
    std::thread mainLoop(&Server::listenLoop, s);
    DEBUG_PRINT(YELLOW, "Entering server main loop");
    s.localLoop();

    mainLoop.join();

    for(int i = 0; i < s.rooms.size(); i++) {
        delete s.rooms[i];
    }

    return 0;
}
