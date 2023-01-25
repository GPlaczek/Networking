#include "waitingroom.h"
#include "ui_waitingroom.h"
#include <QMessageBox>
#include <time.h>

WaitingRoom::WaitingRoom(QWidget *parent, QTcpSocket *socket) : QDialog(parent), ui(new Ui::WaitingRoom) {
    ui->setupUi(this);
    this->socket = socket;
    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, [this]{listUsers();});
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &WaitingRoom::disconnect);
    connect(ui->toLobbyBtn, &QPushButton::clicked, this, &WaitingRoom::toLobby);
    connect(this->socket, &QTcpSocket::readyRead, this, &WaitingRoom::socketReadData);
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

void WaitingRoom::listUsers() {
    ui->usersList->clear();
    QString listUsers = "users";
    QByteArray listUsersUtf8 = (listUsers+'\n').toUtf8();
    imWaiting = true;
    this->socket->write(listUsersUtf8);

    while (1) {
        if(this->socket->waitForReadyRead(3000)) {
            QString items = this->socket->readAll();
            imWaiting = false;
            QString item = "";
            qDebug() << items;
            bool exists = false;
            for (int i = 0; i < items.size(); i++) {
                if(items[i] == ' ') exists = true;
                else if(items[i] == '\n') {
                    if (item == "") return;
                    ui->usersList->addItem(item);
                    item.clear();
                    exists = false;
                } else if (exists == false) {
                    item += items[i];
                }
            }
        }
    }
    imWaiting = false;
}

void WaitingRoom::disconnect(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Disconnect", "Are you sure?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        this->socket->close();
        this->close();
    }
}

void WaitingRoom::toLobby() {
    QString l = "leave";
    QByteArray leaveUtf8 = (l+'\n').toUtf8();
    qDebug() << leaveUtf8;
    this->socket->write(leaveUtf8);

    imWaiting = true; //without this, lobby wouldn't get users list
    this->close();
    this->lobby = new Lobby(nullptr, this->socket);
    this->lobby->show();
}

void WaitingRoom::socketReadData() {
    if(imWaiting == false) {
        QString servMsg = this->socket->readAll();
        qDebug("nasluch");
        qDebug() << servMsg;
        if(servMsg == "start\n") {
            this->close();
            this->roomGame = new RoomGame(nullptr, this->socket);
            this->roomGame->show();
        } else {
            ui->msgText->clear();
            ui->msgText->setAlignment(Qt::AlignCenter);
            ui->msgText->show();
            ui->msgText->append(servMsg);
        }
    }
}
