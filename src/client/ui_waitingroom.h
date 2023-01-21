/********************************************************************************
** Form generated from reading UI file 'waitingroom.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINGROOM_H
#define UI_WAITINGROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_WaitingRoom
{
public:
    QPushButton *refreshUserListBtn;
    QLabel *label_3;
    QListWidget *usersList;
    QLabel *label_6;
    QPushButton *createRoomBtn;
    QTextEdit *msgText;
    QPushButton *disconnectBtn;
    QPushButton *toLobbyBtn;

    void setupUi(QDialog *WaitingRoom)
    {
        if (WaitingRoom->objectName().isEmpty())
            WaitingRoom->setObjectName("WaitingRoom");
        WaitingRoom->resize(780, 791);
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        font.setPointSize(16);
        WaitingRoom->setFont(font);
        refreshUserListBtn = new QPushButton(WaitingRoom);
        refreshUserListBtn->setObjectName("refreshUserListBtn");
        refreshUserListBtn->setGeometry(QRect(40, 700, 321, 41));
        label_3 = new QLabel(WaitingRoom);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(490, 80, 201, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(24);
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        usersList = new QListWidget(WaitingRoom);
        usersList->setObjectName("usersList");
        usersList->setGeometry(QRect(40, 230, 321, 451));
        label_6 = new QLabel(WaitingRoom);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(140, 150, 201, 31));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        createRoomBtn = new QPushButton(WaitingRoom);
        createRoomBtn->setObjectName("createRoomBtn");
        createRoomBtn->setGeometry(QRect(470, 330, 231, 41));
        msgText = new QTextEdit(WaitingRoom);
        msgText->setObjectName("msgText");
        msgText->setEnabled(true);
        msgText->setGeometry(QRect(0, 0, 781, 70));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Lato")});
        font2.setPointSize(18);
        msgText->setFont(font2);
        msgText->setLineWidth(1);
        msgText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgText->setReadOnly(true);
        disconnectBtn = new QPushButton(WaitingRoom);
        disconnectBtn->setObjectName("disconnectBtn");
        disconnectBtn->setGeometry(QRect(470, 230, 231, 41));
        toLobbyBtn = new QPushButton(WaitingRoom);
        toLobbyBtn->setObjectName("toLobbyBtn");
        toLobbyBtn->setGeometry(QRect(470, 280, 231, 41));

        retranslateUi(WaitingRoom);

        QMetaObject::connectSlotsByName(WaitingRoom);
    } // setupUi

    void retranslateUi(QDialog *WaitingRoom)
    {
        WaitingRoom->setWindowTitle(QCoreApplication::translate("WaitingRoom", "Dialog", nullptr));
        refreshUserListBtn->setText(QCoreApplication::translate("WaitingRoom", "Refresh room list", nullptr));
        label_3->setText(QCoreApplication::translate("WaitingRoom", "Waiting room", nullptr));
        label_6->setText(QCoreApplication::translate("WaitingRoom", "Users", nullptr));
        createRoomBtn->setText(QCoreApplication::translate("WaitingRoom", "Start a game", nullptr));
        msgText->setHtml(QCoreApplication::translate("WaitingRoom", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Lato'; font-size:18pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        disconnectBtn->setText(QCoreApplication::translate("WaitingRoom", "Disconnect", nullptr));
        toLobbyBtn->setText(QCoreApplication::translate("WaitingRoom", "Back to lobby", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WaitingRoom: public Ui_WaitingRoom {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGROOM_H
