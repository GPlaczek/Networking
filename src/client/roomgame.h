#pragma once

#include <QDialog>

namespace Ui {
class RoomGame;
}

class RoomGame : public QDialog
{
    Q_OBJECT

public:
    explicit RoomGame(QWidget *parent = nullptr);
    ~RoomGame();

private:
    Ui::RoomGame *ui;
};
