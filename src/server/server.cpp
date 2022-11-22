#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <thread>

#include <sys/socket.h>
#include <sys/epoll.h>

#include <arpa/inet.h>

#define OPTSTRING "hi:p:r:c:q:"
#define N_ROOMS_DEFAULT 16
#define N_CLIENTS_DEFAULT 64
#define QUEUE_LEN_DEFAULT 32

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

    int epollFd;

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
            // TODO: register new user, ask him for username and add him to some array or vector
            int newFd = accept(this -> socketFd, addr, &addrlen);
            struct epoll_event ev = {.events = EPOLLIN, .data= {.u64 = (uint64_t)newFd}};
            epoll_ctl(this -> epollFd, EPOLL_CTL_ADD, newFd, &ev);
        }
    }

    void localLoop() {
        struct epoll_event incomming;
        char buf[256];
        while(1) {
            epoll_wait(this->epollFd, &incomming, 1, -1);
            if (read(incomming.data.u64, buf, 256) > 0) {
                printf("%ld sent: %s\n", incomming.data.u64, buf);
            }
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
                port = htons(port);
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
    std::thread mainLoop(&Server::listenLoop, s);
    s.localLoop();

    mainLoop.join();

    return 0;
}
