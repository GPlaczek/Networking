#include "roomgame.h"
#include "ui_roomgame.h"
#include <QMessageBox>

RoomGame::RoomGame(QWidget *parent, QTcpSocket *socket) : QDialog(parent), ui(new Ui::RoomGame) {
    ui->setupUi(this);
    this->socket = socket;
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
    imWaiting = false;
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

    imWaiting = true; //without this, lobby wouldn't get users list
    this->close();
    this->lobby = new Lobby(nullptr, this->socket);
    this->lobby->show();
}

void RoomGame::socketReadData() {
    if(imWaiting == false) {
        QString servMsg = this->socket->readAll();
        qDebug() << servMsg;
        ui->msgText->clear();
        ui->msgText->setAlignment(Qt::AlignCenter);
        ui->msgText->show();
        ui->msgText->append(servMsg);
    }
}
