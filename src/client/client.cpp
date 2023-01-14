#include "gui.h"

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
    Gui w;
    w.show();
    return app.exec();
}
