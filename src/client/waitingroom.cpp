#include "waitingroom.h"
#include "ui_waitingroom.h"
#include <QMessageBox>

WaitingRoom::WaitingRoom(QWidget *parent, QTcpSocket *socket, QString username) : QDialog(parent), ui(new Ui::WaitingRoom) {
    ui->setupUi(this);
    ui->msgText->hide();
    ui->msgText->setStyleSheet("QTextEdit { background-color : transparent; color : #de0a26; }");

    this->imWaiting = false;
    this->socket = socket;
    this->username = username;

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
    this->imWaiting = true;
    this->socket->write(listUsersUtf8);

    while (1) {
        if(this->socket->waitForReadyRead(3000)) {
            QString items = this->socket->readAll();
            this->imWaiting = false;
            QString item = "";
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
    this->imWaiting = false;
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
    this->socket->write(leaveUtf8);

    this->imWaiting = true; //without this, lobby wouldn't get users list
    this->close();
    this->lobby = new Lobby(nullptr, this->socket, this->username);
    this->lobby->show();
}

void WaitingRoom::socketReadData() {
    if(!this->imWaiting) {
        QString servMsg = this->socket->readAll();
        if(servMsg == "start\n") {
            this->socket->disconnect(this);
            this->close();
            this->roomGame = new RoomGame(nullptr, this->socket, this->username);
            this->roomGame->show();
        } else {
            ui->msgText->clear();
            ui->msgText->setAlignment(Qt::AlignCenter);
            ui->msgText->show();
            ui->msgText->append(servMsg);
        }
    }
}
