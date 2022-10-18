#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstdio>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        const char msg[] = "Unspecified port number\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        return 1;
    }

    unsigned short port;
    sscanf(argv[1], "%hu", &port);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    int one = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int stat = bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (stat != 0) { perror("Could not bind address to the socket"); }
    listen(sock_fd, 2);
    while(1) {
        sockaddr incomming;
        socklen_t size = sizeof(incomming);
        int sck = accept(sock_fd, 0, 0);
        write(sck, "Hello world\n", 13);
    }
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
    return 0;
}

