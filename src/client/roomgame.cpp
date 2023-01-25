#include "roomgame.h"
#include "ui_roomgame.h"
#include <QMessageBox>

RoomGame::RoomGame(QWidget *parent, QTcpSocket *socket, QString username) : QDialog(parent), ui(new Ui::RoomGame) {
    ui->setupUi(this);
    ui->describerText->setReadOnly(true);
    ui->guesserText->setReadOnly(true);

    this->socket = socket;
    this->imWaiting = false;
    this->username = username;

    connect(ui->refreshUserListBtn, &QPushButton::clicked, this, [this]{listUsers();});
    connect(ui->disconnectBtn, &QPushButton::clicked, this, &RoomGame::disconnect);
    connect(ui->toLobbyBtn, &QPushButton::clicked, this, &RoomGame::toLobby);
    connect(ui->sendBtn, &QPushButton::clicked, this, &RoomGame::sendText);
    connect(this->socket, &QTcpSocket::readyRead, this, &RoomGame::socketReadData);
}

RoomGame::~RoomGame() {
    delete ui;
}

void RoomGame::sendText() {
    QString sendLine = "msg " + ui->sendLine->text().trimmed();
    ui->msgText->clear();
    if(sendLine != "") {
        QByteArray sendLineUtf8 = (sendLine+'\n').toUtf8();
        socket->write(sendLineUtf8);
    } else {
        ui->msgText->append("Enter text");
    }
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
    this->lobby = new Lobby(nullptr, this->socket, this->username);
    this->lobby->show();
}

void RoomGame::socketReadData() {
    if(!this->imWaiting) {
        //clock liczba sec do konca
        //win nick - po tym wysłać users
        //start nr rundy, nick opisującego
        //describe slowo do opisania
        //end - koniec gry - wyslij users
        ui->msgText->clear();
        QString servMsg = this->socket->readAll();
        qDebug() << servMsg;
        QString command = servMsg.split(" ").at(0);

        if(command == "clock") {
            QString desc = servMsg.split(" ").at(1);
            ui->timeLeft->append("Time left: " + desc);
        }
        else if (command == "win") {
            ui->wordToDescribe->clear();
            ui->describerText->clear();
        }
        else if (command == "start") {
            QString round = servMsg.split(" ").at(1);
            QString desc = servMsg.split(" ").at(2);
            ui->roundNum->clear();
            ui->roundNum->append("Round " + round);

            QString describer = desc.split("\n").at(0);
            if(describer == this->username) {
                if(desc.length() - describer.length() > 1) { //start and describe command are sticked together
                    servMsg = servMsg.split("\n").at(1);
                    command = servMsg.split(" ").at(0);
                    qDebug() << servMsg << ": " << command;
                }
            } else {
                ui->wordToDescribe->clear();
                ui->wordToDescribe->setAlignment(Qt::AlignCenter);
                ui->wordToDescribe->append("Describer: " + describer);
                ui->wordToDescribe->show();
            }
        }
        else if (command == "end") {
//            RoomGame::listUsers();
        }
        else if (command == "1") { //describer sent message
            QString author = servMsg.split(" ").at(1);
            QString desc = servMsg.split(" ").at(2);
            ui->describerText->appendPlainText("<" + author + "> " + desc + '\n');
        }
        else if (command == "0") { //guesser sent message
            QString author = servMsg.split(" ").at(1);
            QString desc = servMsg.split(" ").at(2);
            ui->guesserText->appendPlainText("<" + author + "> " + desc + '\n');
        }

        if (command == "describe") {
            QString desc = servMsg.split(" ").at(1);
            ui->wordToDescribe->clear();
            ui->wordToDescribe->setAlignment(Qt::AlignCenter);
            ui->wordToDescribe->append("Describe word: " + desc);
            ui->wordToDescribe->show();
        }

//        else {
//            ui->msgText->append(servMsg);
//        }
        ui->msgText->setAlignment(Qt::AlignCenter);
        ui->msgText->show();
    }
}
