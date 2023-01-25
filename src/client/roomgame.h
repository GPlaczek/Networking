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
    explicit RoomGame(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~RoomGame();

protected:
    QTcpSocket *socket;
    void listUsers();
    void disconnect();
    void toLobby();
    void socketReadData();

private:
    bool imWaiting;
    Ui::RoomGame *ui;
    Lobby *lobby;
};
