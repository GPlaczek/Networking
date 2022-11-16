#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#define OPTSTRING "i:p:r:c:q:"
#define N_ROOMS_DEFAULT 16
#define N_CLIENTS_DEFAULT 64
#define QUEUE_LEN_DEFAULT 32

class Server {
    size_t nRooms;
    size_t nClients;

    sockaddr_in *address;

    int socketFd;

public:
    Server(sockaddr_in *addr, size_t nRooms, size_t nClients, size_t queueLen) {
        this -> nRooms = nRooms;
        this -> nClients = nClients;
        this -> address = addr;

        this -> socketFd = socket(AF_INET, SOCK_STREAM, 0);

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
        }
    }

    if (!pPort) {
        fprintf(stderr, "No port specified\n");
        return 1;
    }
    if (!pAddr) {
        fprintf(stderr, "No ip address specified\n");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ipaddr;

    Server s(&addr, rooms, clients, queueLen);

    return 0;
}
