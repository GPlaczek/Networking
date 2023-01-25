#pragma once

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class WaitingRoom;
}

class Lobby;
#include "lobby.h"
class RoomGame;
#include "roomgame.h"

class WaitingRoom : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingRoom(QWidget *parent = nullptr, QTcpSocket *socket = nullptr, QString username = "");
    ~WaitingRoom();

protected:
    QTcpSocket *socket;
    void listUsers();
    void disconnect();
    void toLobby();
    void socketReadData();

private:
    QString username;
    bool imWaiting;
    Ui::WaitingRoom *ui;
    Lobby *lobby;
    RoomGame *roomGame;
};
