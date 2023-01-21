#include "lobby.h"
#include "waitingroom.h"
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
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &Lobby::disconnect);
//    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, &Lobby::listUsers);
    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, [this]{listItems("users", this->ui->usersList);});
    connect(ui->refreshRoomListBtn, &QPushButton::clicked, this, [this]{listItems("rooms", this->ui->roomsList);});

//    connect(this->socket, &QTcpSocket::readyRead, this, &Lobby::socketReadData);
//    connect(this->socket, &QTcpSocket::disconnected, this, &Lobby::socketDisconnected);
    listItems("users", this->ui->usersList);
    listItems("rooms", this->ui->roomsList);
}

Lobby::~Lobby() {
    delete ui;
}

void Lobby::listUsers() {
    ui->usersList->clear();
    QString listUsers = "users";
    QByteArray listUsersUtf8 = (listUsers+'\n').toUtf8();
    this->socket->write(listUsersUtf8);

    if(this->socket->waitForReadyRead(3000)) {
        QString users = this->socket->readAll();
        QString user = "";
        for (int i = 0; i < users.size(); i++) {
            if(users[i] == '\n') {
                ui->usersList->addItem(user);
                user.clear();
            } else {
                user += users[i];
            }
        }
    } else {
        //server didnt sent response
    }
}

void Lobby::listRooms() {
    ui->roomsList->clear();
    QString listRooms = "rooms";
    QByteArray listRoomsUtf8 = (listRooms+'\n').toUtf8();
    this->socket->write(listRoomsUtf8);

    if(this->socket->waitForReadyRead(3000)) {
        QString rooms = this->socket->readAll();
        QString room = "";
        for (int i = 0; i < rooms.size(); i++) {
            if(rooms[i] == '\n') {
                ui->roomsList->addItem(room);
                room.clear();
            } else {
                room += rooms[i];
            }
        }
    } else {
        //server didnt sent response
    }
}

void Lobby::listItems(QString command, QListWidget* itemList) {
    itemList->clear();
    QString listItems = command;
    QByteArray listItemsUtf8 = (listItems+'\n').toUtf8();
    this->socket->write(listItemsUtf8);

    if(this->socket->waitForReadyRead(3000)) {
        QString items = this->socket->readAll();
        std::cout << items.toStdString() << "\n";
        QString item = "";
        for (int i = 0; i < items.size(); i++) {
            if(items[i] == '\n') {
                itemList->addItem(item);
                item.clear();
            } else {
                item += items[i];
            }
        }
    } else {
        //server didnt sent response
    }
}

void Lobby::createRoom() {
    ui->msgText->clear();
    ui->msgText->setAlignment(Qt::AlignCenter);
    QString roomName = ui->roomName->text().trimmed();

    if(roomName != "") {
        ui->msgText->hide();
        QByteArray roomNameUtf8 = ("create "+roomName+'\n').toUtf8();
        this->socket->write(roomNameUtf8);

        if(this->socket->waitForReadyRead(3000)) {
            QString servMsg = this->socket->readAll();
            std::cout << servMsg.toStdString() << "\n";

            this->close();
            waitingRoom = new WaitingRoom(nullptr);
            waitingRoom->show();
        } else {
            //server didnt sent response
        }

    } else {
        ui->msgText->show();
        ui->msgText->append("Enter room name!");
    }
}

//TODO: dont steal data when someone is waiting for it
void Lobby::socketReadData() {
    QString servMsg = this->socket->readAll();

    ui->msgText->clear();
    ui->msgText->setAlignment(Qt::AlignCenter);
    ui->msgText->show();
    ui->msgText->append(servMsg);
}

void Lobby::disconnect(){
    //TODO: are you sure question
    this->socket->close();
    this->close();
}
