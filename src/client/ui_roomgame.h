/********************************************************************************
** Form generated from reading UI file 'roomgame.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROOMGAME_H
#define UI_ROOMGAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_RoomGame
{
public:
    QPushButton *refreshUserListBtn;
    QPushButton *refreshRoomListBtn;
    QTextEdit *msgText;
    QPushButton *disconnectBtn;
    QPushButton *toLobbyBtn;
    QPlainTextEdit *describerText;
    QLineEdit *describerLine;
    QLineEdit *guesserLine;
    QPlainTextEdit *guesserText;

    void setupUi(QDialog *RoomGame)
    {
        if (RoomGame->objectName().isEmpty())
            RoomGame->setObjectName("RoomGame");
        RoomGame->resize(1041, 816);
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        font.setPointSize(16);
        RoomGame->setFont(font);
        refreshUserListBtn = new QPushButton(RoomGame);
        refreshUserListBtn->setObjectName("refreshUserListBtn");
        refreshUserListBtn->setGeometry(QRect(40, 750, 441, 41));
        refreshRoomListBtn = new QPushButton(RoomGame);
        refreshRoomListBtn->setObjectName("refreshRoomListBtn");
        refreshRoomListBtn->setGeometry(QRect(560, 750, 441, 41));
        msgText = new QTextEdit(RoomGame);
        msgText->setObjectName("msgText");
        msgText->setEnabled(true);
        msgText->setGeometry(QRect(0, 10, 1041, 70));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(18);
        msgText->setFont(font1);
        msgText->setLineWidth(1);
        msgText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgText->setReadOnly(true);
        disconnectBtn = new QPushButton(RoomGame);
        disconnectBtn->setObjectName("disconnectBtn");
        disconnectBtn->setGeometry(QRect(10, 100, 231, 41));
        toLobbyBtn = new QPushButton(RoomGame);
        toLobbyBtn->setObjectName("toLobbyBtn");
        toLobbyBtn->setGeometry(QRect(260, 100, 231, 41));
        describerText = new QPlainTextEdit(RoomGame);
        describerText->setObjectName("describerText");
        describerText->setGeometry(QRect(40, 180, 441, 461));
        describerLine = new QLineEdit(RoomGame);
        describerLine->setObjectName("describerLine");
        describerLine->setGeometry(QRect(560, 670, 441, 51));
        describerLine->setFont(font);
        guesserLine = new QLineEdit(RoomGame);
        guesserLine->setObjectName("guesserLine");
        guesserLine->setGeometry(QRect(40, 670, 441, 51));
        guesserLine->setFont(font);
        guesserText = new QPlainTextEdit(RoomGame);
        guesserText->setObjectName("guesserText");
        guesserText->setGeometry(QRect(560, 180, 441, 461));

        retranslateUi(RoomGame);

        QMetaObject::connectSlotsByName(RoomGame);
    } // setupUi

    void retranslateUi(QDialog *RoomGame)
    {
        RoomGame->setWindowTitle(QCoreApplication::translate("RoomGame", "Dialog", nullptr));
        refreshUserListBtn->setText(QCoreApplication::translate("RoomGame", "Send description", nullptr));
        refreshRoomListBtn->setText(QCoreApplication::translate("RoomGame", "Send guess", nullptr));
        msgText->setHtml(QCoreApplication::translate("RoomGame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Lato'; font-size:18pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        disconnectBtn->setText(QCoreApplication::translate("RoomGame", "Disconnect", nullptr));
        toLobbyBtn->setText(QCoreApplication::translate("RoomGame", "Back to lobby", nullptr));
        describerText->setPlainText(QCoreApplication::translate("RoomGame", "gf", nullptr));
        guesserText->setPlainText(QCoreApplication::translate("RoomGame", " vngfg", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RoomGame: public Ui_RoomGame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROOMGAME_H
