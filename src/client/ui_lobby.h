/********************************************************************************
** Form generated from reading UI file 'lobby.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOBBY_H
#define UI_LOBBY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Lobby
{
public:
    QPushButton *refreshUserListBtn;
    QListWidget *usersList;
    QPushButton *createRoomBtn;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *roomName;
    QTextEdit *msgText;
    QPushButton *disconnectBtn;
    QPushButton *refreshRoomListBtn;
    QLabel *label_5;
    QLabel *label_6;
    QListWidget *roomsList;
    QSpinBox *maxPlayers;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *nRounds;
    QLabel *label_9;
    QSpinBox *roundTime;
    QLabel *label_10;

    void setupUi(QDialog *Lobby)
    {
        if (Lobby->objectName().isEmpty())
            Lobby->setObjectName("Lobby");
        Lobby->resize(1034, 861);
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        font.setPointSize(16);
        Lobby->setFont(font);
        refreshUserListBtn = new QPushButton(Lobby);
        refreshUserListBtn->setObjectName("refreshUserListBtn");
        refreshUserListBtn->setGeometry(QRect(40, 720, 321, 41));
        usersList = new QListWidget(Lobby);
        usersList->setObjectName("usersList");
        usersList->setGeometry(QRect(40, 250, 321, 451));
        createRoomBtn = new QPushButton(Lobby);
        createRoomBtn->setObjectName("createRoomBtn");
        createRoomBtn->setGeometry(QRect(770, 720, 231, 41));
        label = new QLabel(Lobby);
        label->setObjectName("label");
        label->setGeometry(QRect(400, 210, 331, 31));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(Lobby);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(770, 230, 231, 31));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(Lobby);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(490, 100, 201, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(24);
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(Lobby);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(770, 340, 231, 31));
        label_4->setAlignment(Qt::AlignCenter);
        roomName = new QLineEdit(Lobby);
        roomName->setObjectName("roomName");
        roomName->setGeometry(QRect(770, 380, 231, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Lato")});
        font2.setPointSize(14);
        roomName->setFont(font2);
        roomName->setMaxLength(20);
        roomName->setCursorPosition(0);
        msgText = new QTextEdit(Lobby);
        msgText->setObjectName("msgText");
        msgText->setEnabled(true);
        msgText->setGeometry(QRect(0, 20, 1041, 70));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Lato")});
        font3.setPointSize(18);
        msgText->setFont(font3);
        msgText->setLineWidth(1);
        msgText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgText->setReadOnly(true);
        disconnectBtn = new QPushButton(Lobby);
        disconnectBtn->setObjectName("disconnectBtn");
        disconnectBtn->setGeometry(QRect(770, 160, 231, 41));
        refreshRoomListBtn = new QPushButton(Lobby);
        refreshRoomListBtn->setObjectName("refreshRoomListBtn");
        refreshRoomListBtn->setGeometry(QRect(400, 720, 321, 41));
        label_5 = new QLabel(Lobby);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(490, 160, 201, 31));
        label_5->setFont(font1);
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_6 = new QLabel(Lobby);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(140, 170, 201, 31));
        label_6->setFont(font1);
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        roomsList = new QListWidget(Lobby);
        roomsList->setObjectName("roomsList");
        roomsList->setGeometry(QRect(400, 250, 321, 451));
        maxPlayers = new QSpinBox(Lobby);
        maxPlayers->setObjectName("maxPlayers");
        maxPlayers->setGeometry(QRect(770, 470, 231, 41));
        maxPlayers->setMinimum(3);
        maxPlayers->setMaximum(10);
        label_7 = new QLabel(Lobby);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(770, 440, 231, 31));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(Lobby);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(770, 530, 231, 31));
        label_8->setAlignment(Qt::AlignCenter);
        nRounds = new QSpinBox(Lobby);
        nRounds->setObjectName("nRounds");
        nRounds->setGeometry(QRect(770, 560, 231, 41));
        nRounds->setMinimum(1);
        nRounds->setMaximum(10);
        label_9 = new QLabel(Lobby);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(770, 620, 231, 31));
        label_9->setAlignment(Qt::AlignCenter);
        roundTime = new QSpinBox(Lobby);
        roundTime->setObjectName("roundTime");
        roundTime->setGeometry(QRect(770, 650, 231, 41));
        roundTime->setMinimum(20);
        roundTime->setMaximum(600);
        label_10 = new QLabel(Lobby);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(750, 290, 271, 31));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("Lato")});
        font4.setPointSize(20);
        label_10->setFont(font4);
        label_10->setAlignment(Qt::AlignCenter);

        retranslateUi(Lobby);

        QMetaObject::connectSlotsByName(Lobby);
    } // setupUi

    void retranslateUi(QDialog *Lobby)
    {
        Lobby->setWindowTitle(QCoreApplication::translate("Lobby", "Dialog", nullptr));
        refreshUserListBtn->setText(QCoreApplication::translate("Lobby", "Refresh users list", nullptr));
        createRoomBtn->setText(QCoreApplication::translate("Lobby", "Create a new room", nullptr));
        label->setText(QCoreApplication::translate("Lobby", "Click on room to join it", nullptr));
        label_2->setText(QCoreApplication::translate("Lobby", "Nothing interesting?", nullptr));
        label_3->setText(QCoreApplication::translate("Lobby", "Lobby", nullptr));
        label_4->setText(QCoreApplication::translate("Lobby", "Name your room", nullptr));
        roomName->setInputMask(QString());
        roomName->setText(QString());
        roomName->setPlaceholderText(QCoreApplication::translate("Lobby", "Max 20 charackers", nullptr));
        msgText->setHtml(QCoreApplication::translate("Lobby", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Lato'; font-size:18pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        disconnectBtn->setText(QCoreApplication::translate("Lobby", "Disconnect", nullptr));
        refreshRoomListBtn->setText(QCoreApplication::translate("Lobby", "Refresh rooms list", nullptr));
        label_5->setText(QCoreApplication::translate("Lobby", "Rooms", nullptr));
        label_6->setText(QCoreApplication::translate("Lobby", "Users", nullptr));
        label_7->setText(QCoreApplication::translate("Lobby", "Max number of players", nullptr));
        label_8->setText(QCoreApplication::translate("Lobby", "Number of rounds", nullptr));
        label_9->setText(QCoreApplication::translate("Lobby", "Round time in seconds", nullptr));
        label_10->setText(QCoreApplication::translate("Lobby", "Create your own room", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Lobby: public Ui_Lobby {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBY_H
