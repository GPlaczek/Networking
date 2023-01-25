#include "lobby.h"
#include "ui_lobby.h"
#include "waitingroom.h"

#include <unistd.h>
#include <iostream>
#include <string>
#include <QMessageBox>

Lobby::Lobby(QWidget *parent, QTcpSocket *socket, QString username) : QDialog(parent), ui(new Ui::Lobby) {
    ui->setupUi(this);
    ui->msgText->hide();
    ui->msgText->setStyleSheet("QTextEdit { background-color : transparent; color : #de0a26; }");

    this->username = username;
    this->socket = socket;

    connect(ui->createRoomBtn, &QPushButton::clicked, this, &Lobby::createRoom);
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &Lobby::disconnect);
    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, [this]{listItems("users", this->ui->usersList);});
    connect(ui->refreshRoomListBtn, &QPushButton::clicked, this, [this]{listItems("rooms", this->ui->roomsList);});

    listItems("users", this->ui->usersList);
    listItems("rooms", this->ui->roomsList);
    connect(ui->roomsList, &QListWidget::itemClicked, this, [this](QListWidgetItem *pItem){joinRoom(pItem);});
}

Lobby::~Lobby() {
    delete ui;
}

void Lobby::listItems(QString command, QListWidget* itemList) {
    itemList->clear();
    QString listItems = command;
    QByteArray listItemsUtf8 = (listItems+'\n').toUtf8();
    this->socket->write(listItemsUtf8);

    while (1) {
        if(this->socket->waitForReadyRead(3000)) {
            QString items = this->socket->readAll();
            QString item = "";
            for (int i = 0; i < items.size(); i++) {
                if(items[i] == '\n') {
                    if (item == "") return;
                    itemList->addItem(item);
                    item.clear();
                } else {
                    item += items[i];
                }
            }
        }
    }
}

void Lobby::joinRoom(QListWidgetItem *item) {
    //extract room index (first number) from the whole room info
    QString roomIndex = item->text().mid(0, item->text().indexOf(" ")).trimmed();
    QByteArray roomIndexUtf8 = ("join "+roomIndex+'\n').toUtf8();
    this->socket->write(roomIndexUtf8);

    if(this->socket->waitForReadyRead(3000)) {
        QString servMsg = this->socket->readAll();
        qDebug() << servMsg;

        if(servMsg == "0\n") {
            this->close();
            this->waitingRoom = new WaitingRoom(nullptr, this->socket, this->username);
            this->waitingRoom->show();
        } else if(servMsg == "start\n0\n") { //third person in room - game starts
            this->close();
            this->roomGame = new RoomGame(nullptr, this->socket, this->username);
            this->roomGame->show();
        } else {
            ui->msgText->show();
            ui->msgText->append("Could not join the room");
        }
    }
}

void Lobby::createRoom() {
    ui->msgText->clear();
    ui->msgText->setAlignment(Qt::AlignCenter);
    QString roomName = ui->roomName->text().trimmed();
    QString maxPlayers = ui->maxPlayers->text();
    QString nRounds = ui->nRounds->text();
    QString roundTime = ui->roundTime->text();

    qDebug() << maxPlayers << " " << nRounds << " " << roundTime << "\n";

    if(roomName != "") {
        ui->msgText->hide();
        QByteArray roomNameUtf8 = ("create "+roomName+' '+maxPlayers+' '+nRounds+' '+roundTime+'\n').toUtf8();
        this->socket->write(roomNameUtf8);

        if(this->socket->waitForReadyRead(3000)) {
            QString servMsg = this->socket->readAll();
            qDebug() << servMsg;
            if(servMsg == "0\n") {
                this->close();
                this->waitingRoom = new WaitingRoom(nullptr, this->socket, this->username);
                this->waitingRoom->show();
            } else {
                ui->msgText->show();
                ui->msgText->append("Could not create the room");
            }
        }
    } else {
        ui->msgText->show();
        ui->msgText->append("Enter room name!");
    }
}

void Lobby::disconnect(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Disconnect", "Are you sure?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        this->socket->close();
        this->close();
    }
}
