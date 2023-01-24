#pragma once

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>
#include <QListWidget>
#include <sys/socket.h>

namespace Ui {
class Lobby;
}

class WaitingRoom;
#include "waitingroom.h"

class Lobby : public QDialog
{
    Q_OBJECT

public:
    explicit Lobby(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~Lobby();

protected:
    QTcpSocket *socket;
    void listItems(QString command, QListWidget* itemList);
    void joinRoom(QListWidgetItem *item);
    void createRoom();
    void disconnect();
    void socketDisconnected();
    void socketReadData();

private:
    Ui::Lobby *ui;
    WaitingRoom *waitingRoom;
};
