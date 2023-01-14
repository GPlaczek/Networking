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
#include <QtWidgets/QTextEdit>
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
    QTextEdit *msgText;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName("Gui");
        Gui->setEnabled(true);
        Gui->resize(715, 684);
        Gui->setMinimumSize(QSize(715, 684));
        Gui->setMaximumSize(QSize(715, 684));
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        Gui->setFont(font);
        centralwidget = new QWidget(Gui);
        centralwidget->setObjectName("centralwidget");
        confirmBtn = new QPushButton(centralwidget);
        confirmBtn->setObjectName("confirmBtn");
        confirmBtn->setGeometry(QRect(150, 570, 401, 51));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(16);
        confirmBtn->setFont(font1);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(150, 390, 401, 71));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Lato")});
        font2.setPointSize(24);
        label->setFont(font2);
        label->setAlignment(Qt::AlignCenter);
        username = new QLineEdit(centralwidget);
        username->setObjectName("username");
        username->setGeometry(QRect(150, 460, 401, 51));
        username->setFont(font1);
        port = new QSpinBox(centralwidget);
        port->setObjectName("port");
        port->setGeometry(QRect(150, 300, 401, 51));
        port->setFont(font1);
        port->setMinimum(1025);
        port->setMaximum(65535);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(150, 100, 401, 61));
        label_4->setFont(font2);
        label_4->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 240, 401, 61));
        label_2->setFont(font2);
        label_2->setAlignment(Qt::AlignCenter);
        ip = new QLineEdit(centralwidget);
        ip->setObjectName("ip");
        ip->setGeometry(QRect(150, 160, 401, 51));
        ip->setFont(font1);
        msgText = new QTextEdit(centralwidget);
        msgText->setObjectName("msgText");
        msgText->setEnabled(true);
        msgText->setGeometry(QRect(0, 30, 721, 70));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Lato")});
        font3.setPointSize(18);
        msgText->setFont(font3);
        msgText->setLineWidth(1);
        msgText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgText->setReadOnly(true);
        Gui->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Gui);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 715, 20));
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
        msgText->setHtml(QCoreApplication::translate("Gui", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Lato'; font-size:18pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_H
