#include "gui.h"
#include "lobby.h"
#include "./ui_gui.h"

#include <unistd.h>
#include <iostream>
#include <QMessageBox>

Gui::Gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::Gui) {
    ui->setupUi(this);
    connect(ui->confirmBtn, &QPushButton::clicked, this, &Gui::connectToServer);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &Gui::socketConnected);

//    connect(socket, &QTcpSocket::readyRead, this, &Gui::socketReadData());
//    connect(socket, &QTcpSocket::disconnected, this, &Gui::socketDisconnected());
//    connect(socket, &QTcpSocket::errorOccurred, this, &Gui::socketError());
}

Gui::~Gui() {
    delete ui;
}

void Gui::connectToServer(){
    QString ip = ui->ip->text();
    int port = ui->port->value();
    socket->connectToHost(ip, port);

    connect(connTimeoutTimer, &QTimer::timeout, [&]{
        socket->abort();
        connTimeoutTimer->disconnect();
        QMessageBox::critical(this, "Error", "Connect timed out");
    });
    connTimeoutTimer->start(3000);

    if(socket->state() == QAbstractSocket::ConnectingState || socket->state() == QAbstractSocket::ConnectedState) {
        QString username = ui->username->text().trimmed();
        QByteArray usernameUtf8 = (username+'\n').toUtf8();
        socket->write(usernameUtf8); //send username to server

        this->close();
        lobby = new Lobby();
        lobby->show();
    }
}

void Gui::socketConnected(){
    connTimeoutTimer->stop();
    connTimeoutTimer->disconnect();
}

