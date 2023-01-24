#include "waitingroom.h"
#include "ui_waitingroom.h"
#include <QMessageBox>

WaitingRoom::WaitingRoom(QWidget *parent, QTcpSocket *socket) : QDialog(parent), ui(new Ui::WaitingRoom) {
    ui->setupUi(this);
    this->socket = socket;
    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, [this]{listUsers();});
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &WaitingRoom::disconnect);
    connect(ui->toLobbyBtn, &QPushButton::clicked, this, &WaitingRoom::toLobby);
    connect(this->socket, &QTcpSocket::readyRead, this, &WaitingRoom::socketReadData);
    //TODO: Back to Lobby

}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

void WaitingRoom::listUsers() {
    ui->usersList->clear();
    QString listUsers = "users";
    QByteArray listUsersUtf8 = (listUsers+'\n').toUtf8();
    this->socket->write(listUsersUtf8);

    while (1) {
        if(this->socket->waitForReadyRead(3000)) {
            QString items = this->socket->readAll();
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

    this->close();
    this->lobby = new Lobby(nullptr, this->socket);
    this->lobby->show();
}

void WaitingRoom::socketReadData() {
    if(this->socket->waitForReadyRead(300)) {
        QString servMsg = this->socket->readAll();
        qDebug() << servMsg;
        ui->msgText->clear();
        ui->msgText->setAlignment(Qt::AlignCenter);
        ui->msgText->show();
        ui->msgText->append(servMsg);
    }
}
