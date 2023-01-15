#include "lobby.h"
#include "ui_lobby.h"

#include <unistd.h>
#include <iostream>
#include <string>
#include <QMessageBox>

Lobby::Lobby(QWidget *parent, QTcpSocket *socket) : QDialog(parent), ui(new Ui::Lobby) {
    ui->setupUi(this);
    ui->msgText->hide();
    ui->msgText->setStyleSheet("QTextEdit { background-color : transparent; color : #de0a26; }");

    this->socket = socket;
    connect(ui->createRoomBtn, &QPushButton::clicked, this, &Lobby::createRoom);
    connect(this->socket, &QTcpSocket::readyRead, this, &Lobby::socketReadData);
}

Lobby::~Lobby() {
    delete ui;
}

void Lobby::createRoom() {
    ui->msgText->clear();
    ui->msgText->setAlignment(Qt::AlignCenter);
    QString roomName = ui->roomName->text().trimmed();

    if(roomName != "") {
        ui->msgText->hide();
        QByteArray roomNameUtf8 = ("create "+roomName+'\n').toUtf8();
        this->socket->write(roomNameUtf8);
    } else {
        ui->msgText->show();
        ui->msgText->append("Enter room name!");
    }
}

void Lobby::socketReadData() {
    QString servMsg = this->socket->read(256);

    ui->msgText->clear();
    ui->msgText->setAlignment(Qt::AlignCenter);
    ui->msgText->show();
    ui->msgText->append(servMsg);
}
