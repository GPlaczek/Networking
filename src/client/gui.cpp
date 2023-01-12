#include "gui.hpp"

#include <QMessageBox>
#include <QLabel>
#include <QPushButton>

Window::Window(QWidget *parent) : QWidget(parent) {
    setFixedSize(500, 250);

    label = new QLabel("Enter username", this);
    btn = new QPushButton("Submit", this);
    btn->setGeometry(10, 10, 80, 30);
}

Window::~Window(){}