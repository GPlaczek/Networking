#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <vector>
#include <string>
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
};

class Server {
    size_t nRooms;
    size_t nClients;

    sockaddr_in *address;

    int socketFd;

    int epollFd;
    std::vector <Client*> clients;

    void registerUser(int newFd, sockaddr *addr) {
        struct epoll_event ev = {.events = EPOLLIN, .data= {.u64 = (uint64_t)newFd}};
        epoll_ctl(this -> epollFd, EPOLL_CTL_ADD, newFd, &ev);

        Client *client = new Client;
        client -> socketDesc = newFd;
        client -> address = (sockaddr_in*)addr;
        
        char buf[256];
        if (read(newFd, buf, 256) > 0) {
            client -> username = buf;
        }
        clients.push_back(client);
    }

public:
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
            registerUser(newFd, addr);
            DEBUG_PRINT(RED, "New user registered");
        }
    }

    void localLoop() {
        struct epoll_event incomming;
        char buf[256];
        while(1) {
            DEBUG_PRINT(BLUE, "Polling on existing connections");
            epoll_wait(this->epollFd, &incomming, 1, -1);
            if (read(incomming.data.u64, buf, 256) > 0) {
                PPRINTF(BLUE, "%ld sent: %s", incomming.data.u64, buf);
            }
            memset(buf, 0, 256);
            write(incomming.data.u64, "leave me alone", 14);
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

    return 0;
}
