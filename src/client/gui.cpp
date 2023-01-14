#include "gui.h"
#include "lobby.h"
#include "./ui_gui.h"

#include <unistd.h>
#include <iostream>
#include <QMessageBox>

Gui::Gui(QWidget *parent) : QMainWindow(parent), ui(new Ui::Gui) {
    ui->setupUi(this);
    ui->msgText->hide();
    ui->msgText->setStyleSheet("QTextEdit { background-color : transparent; color : #de0a26; }");

    connect(ui->confirmBtn, &QPushButton::clicked, this, &Gui::connectToServer);
    socket = new QTcpSocket(this);
}

Gui::~Gui() {
    delete ui;
}

void Gui::connectToServer(){
    ui->msgText->clear();
    ui->msgText->setAlignment(Qt::AlignCenter);
    
    QString username = ui->username->text().trimmed();
    QString ip = ui->ip->text();
    int port = ui->port->value();
    if(ip == "") {
        ui->msgText->show();
        ui->msgText->append("Enter ip address!");
    }
    else if(username == "") {
        ui->msgText->show();
        ui->msgText->append("Enter username!");
    }
    else {
        ui->msgText->hide();
        socket->connectToHost(ip, port);

        if(!socket->waitForConnected(3000))
        {
           socket->abort();
           QMessageBox::critical(this, "Error", "Connect timed out");
        }

        if(socket->state() == QAbstractSocket::ConnectedState) {
            QByteArray usernameUtf8 = (username+'\n').toUtf8();
            socket->write(usernameUtf8); //send username to server

            this->close();
            lobby = new Lobby();
            lobby->show();
        }
    }
}
