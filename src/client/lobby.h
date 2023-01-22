#ifndef LOBBY_H
#define LOBBY_H

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>
#include <QListWidget>
#include <sys/socket.h>
#include "waitingroom.h"

namespace Ui {
class Lobby;
}

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

#endif // LOBBY_H
