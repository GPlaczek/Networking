#pragma once

#include <QWidget>

class QPushButton;
class QLabel;
class Window : public QWidget
{
    Q_OBJECT
    public:
        explicit Window(QWidget *parent = 0);
        ~Window();

    private:
        QPushButton *btn;
        QLabel *label;
};
