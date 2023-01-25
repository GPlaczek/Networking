#include "roomgame.h"
#include "ui_roomgame.h"

RoomGame::RoomGame(QWidget *parent, QTcpSocket *socket) : QDialog(parent), ui(new Ui::RoomGame)
{
    ui->setupUi(this);
}

RoomGame::~RoomGame()
{
    delete ui;
}
