#ifndef LOBBY_H
#define LOBBY_H

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>
#include <sys/socket.h>

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
    void createRoom();
    void socketReadData();

private:
    Ui::Lobby *ui;
};

#endif // LOBBY_H
