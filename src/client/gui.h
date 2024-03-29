#pragma once

#include "lobby.h"
#include <QMainWindow>
#include <QTcpSocket>
#include <sys/socket.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Gui; }
QT_END_NAMESPACE

class Gui : public QMainWindow
{
    Q_OBJECT

public:
    Gui(QWidget *parent = nullptr);
    ~Gui();

protected:
    QTcpSocket *socket;
    void connectToServer();

private:
    Ui::Gui *ui;
    Lobby *lobby;
};
