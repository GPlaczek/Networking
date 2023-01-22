#ifndef ROOMGAME_H
#define ROOMGAME_H

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

#endif // ROOMGAME_H
