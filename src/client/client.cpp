#include "gui.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstdio>
#include <string.h>
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    app.exec();

    if (argc < 3) {
        const char usage[] = "Usage:\n\t";
        write(STDERR_FILENO, usage, sizeof(usage));
        write(STDERR_FILENO, argv[0], strlen(argv[0]));
        const char msg[] = " <ipv4 address> <port number> \n";
        write(STDERR_FILENO, msg, sizeof(msg));
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_aton(argv[1], &addr.sin_addr);
    sscanf(argv[2], "%hu", &addr.sin_port);
    addr.sin_port = htons(addr.sin_port);

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("Could not connect to the given address");
    }
    char buf[256];
    read(sock_fd, &buf, 256);
    printf("%s\n", buf);
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);
    return 0;
}
