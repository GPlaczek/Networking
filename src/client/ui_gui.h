/********************************************************************************
** Form generated from reading UI file 'gui.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_H
#define UI_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QWidget *centralwidget;
    QPushButton *confirmBtn;
    QLabel *label;
    QLineEdit *username;
    QSpinBox *port;
    QLabel *label_4;
    QLabel *label_2;
    QLineEdit *ip;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName("Gui");
        Gui->resize(715, 662);
        centralwidget = new QWidget(Gui);
        centralwidget->setObjectName("centralwidget");
        confirmBtn = new QPushButton(centralwidget);
        confirmBtn->setObjectName("confirmBtn");
        confirmBtn->setGeometry(QRect(150, 530, 401, 51));
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        font.setPointSize(16);
        confirmBtn->setFont(font);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(150, 350, 401, 71));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(24);
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        username = new QLineEdit(centralwidget);
        username->setObjectName("username");
        username->setGeometry(QRect(150, 420, 401, 51));
        username->setFont(font);
        port = new QSpinBox(centralwidget);
        port->setObjectName("port");
        port->setGeometry(QRect(150, 260, 401, 51));
        port->setFont(font);
        port->setMinimum(1025);
        port->setMaximum(65535);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(150, 60, 401, 61));
        label_4->setFont(font1);
        label_4->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 200, 401, 61));
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);
        ip = new QLineEdit(centralwidget);
        ip->setObjectName("ip");
        ip->setGeometry(QRect(150, 120, 401, 51));
        ip->setFont(font);
        Gui->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Gui);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 715, 22));
        Gui->setMenuBar(menubar);
        statusbar = new QStatusBar(Gui);
        statusbar->setObjectName("statusbar");
        Gui->setStatusBar(statusbar);

        retranslateUi(Gui);

        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QMainWindow *Gui)
    {
        Gui->setWindowTitle(QCoreApplication::translate("Gui", "Gui", nullptr));
        confirmBtn->setText(QCoreApplication::translate("Gui", "Confirm", nullptr));
        label->setText(QCoreApplication::translate("Gui", "Enter username", nullptr));
        label_4->setText(QCoreApplication::translate("Gui", "IP address", nullptr));
        label_2->setText(QCoreApplication::translate("Gui", "Port number", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
