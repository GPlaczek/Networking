#pragma once

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class RoomGame;
}

class Lobby;
#include "lobby.h"

class RoomGame : public QDialog
{
    Q_OBJECT

public:
    explicit RoomGame(QWidget *parent = nullptr, QTcpSocket *socket = nullptr, QString username = "");
    ~RoomGame();

protected:
    QTcpSocket *socket;
    void listUsers();
    void disconnect();
    void toLobby();
    void socketReadData();
    void sendText();

private:
    QString username;
    bool imWaiting;
    Ui::RoomGame *ui;
    Lobby *lobby;
};
