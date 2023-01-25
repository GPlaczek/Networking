#pragma once

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class RoomGame;
}

class RoomGame : public QDialog
{
    Q_OBJECT

public:
    explicit RoomGame(QWidget *parent = nullptr, QTcpSocket *socket = nullptr);
    ~RoomGame();

protected:
    QTcpSocket *socket;

private:
    Ui::RoomGame *ui;
};
