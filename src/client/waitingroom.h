#pragma once

#include <QDialog>
#include <QTcpSocket>
#include "roomgame.h"

namespace Ui {
class WaitingRoom;
}

class Lobby;
#include "lobby.h"

class WaitingRoom : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingRoom(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~WaitingRoom();

protected:
    QTcpSocket *socket;
    void listUsers();
    void disconnect();
    void toLobby();
    void socketReadData();

private:
    bool imWaiting = false;
    Ui::WaitingRoom *ui;
    Lobby *lobby;
    RoomGame *roomGame;
};
