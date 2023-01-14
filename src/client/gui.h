#ifndef GUI_H
#define GUI_H

#include "lobby.h"
#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
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
#endif // GUI_H
