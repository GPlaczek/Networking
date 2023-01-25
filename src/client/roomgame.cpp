#include "roomgame.h"
#include "ui_roomgame.h"
#include <QMessageBox>

RoomGame::RoomGame(QWidget *parent, QTcpSocket *socket) : QDialog(parent), ui(new Ui::RoomGame) {
    ui->setupUi(this);

    this->socket = socket;
    this->imWaiting = false;

    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, [this]{listUsers();});
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &RoomGame::disconnect);
    connect(ui->toLobbyBtn, &QPushButton::clicked, this, &RoomGame::toLobby);
    connect(this->socket, &QTcpSocket::readyRead, this, &RoomGame::socketReadData);
}

RoomGame::~RoomGame() {
    delete ui;
}

void RoomGame::listUsers() {
    ui->usersList->clear();
    QString listUsers = "users";
    QByteArray listUsersUtf8 = (listUsers+'\n').toUtf8();
    this->imWaiting = true;
    this->socket->write(listUsersUtf8);

    while (1) {
        if(this->socket->waitForReadyRead(3000)) {
            QString items = this->socket->readAll();
            this->imWaiting = false;
            QString item = "";
            qDebug() << items;
            bool exists = false;
            for (int i = 0; i < items.size(); i++) {
                if(items[i] == '\n') {
                    if (item == "") return;
                    ui->usersList->addItem(item);
                    item.clear();
                    item = QString::number(i) + ". ";
                    exists = false;
                } else if (exists == false) {
                    item += items[i];
                }
            }
        }
    }
    this->imWaiting = false;
}

void RoomGame::disconnect(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Disconnect", "Are you sure?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        this->socket->close();
        this->close();
    }
}

void RoomGame::toLobby() {
    QString l = "leave";
    QByteArray leaveUtf8 = (l+'\n').toUtf8();
    qDebug() << leaveUtf8;
    this->socket->write(leaveUtf8);

    this->imWaiting = true; //without this, lobby wouldn't get users list
    this->close();
    this->lobby = new Lobby(nullptr, this->socket);
    this->lobby->show();
}

void RoomGame::socketReadData() {
    if(!this->imWaiting) {
        QString servMsg = this->socket->readAll();
//        qDebug() << servMsg;
        QString command = servMsg.split(" ").at(0);
        QString desc = servMsg.split(" ").at(1);
        qDebug() << command << " | " << desc;

        if(command == "clock") {

        } else if (command == "win") {

        } else if (command == "start") {

        } else if (command == "describe") {

        } else if (command == "end") {

        }

        //clock liczba sec do konca
        //win nick - po tym wysłać users
        //start nr rundy, nick opisującego
        //describe slowo do opisania
        //end - koniec gry - wyslij users
        ui->msgText->clear();
        ui->msgText->setAlignment(Qt::AlignCenter);
        ui->msgText->show();
        ui->msgText->append(servMsg);
    }
}
