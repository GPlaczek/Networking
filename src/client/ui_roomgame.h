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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_RoomGame
{
public:
    QPushButton *sendDescBtn;
    QPushButton *sendGuessBtn;
    QTextEdit *timeLeft;
    QPushButton *disconnectBtn;
    QPushButton *toLobbyBtn;
    QPlainTextEdit *describerText;
    QLineEdit *describerLine;
    QLineEdit *guesserLine;
    QPlainTextEdit *guesserText;
    QPushButton *refreshUserListBtn;
    QListWidget *usersList;
    QTextEdit *wordToDescribe;
    QTextEdit *msgText;
    QTextEdit *roundNum;

    void setupUi(QDialog *RoomGame)
    {
        if (RoomGame->objectName().isEmpty())
            RoomGame->setObjectName("RoomGame");
        RoomGame->resize(1162, 867);
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        font.setPointSize(16);
        RoomGame->setFont(font);
        sendDescBtn = new QPushButton(RoomGame);
        sendDescBtn->setObjectName("sendDescBtn");
        sendDescBtn->setGeometry(QRect(30, 760, 381, 41));
        sendGuessBtn = new QPushButton(RoomGame);
        sendGuessBtn->setObjectName("sendGuessBtn");
        sendGuessBtn->setGeometry(QRect(450, 760, 371, 41));
        timeLeft = new QTextEdit(RoomGame);
        timeLeft->setObjectName("timeLeft");
        timeLeft->setEnabled(true);
        timeLeft->setGeometry(QRect(890, 0, 281, 70));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(18);
        timeLeft->setFont(font1);
        timeLeft->setLineWidth(1);
        timeLeft->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        timeLeft->setReadOnly(true);
        disconnectBtn = new QPushButton(RoomGame);
        disconnectBtn->setObjectName("disconnectBtn");
        disconnectBtn->setGeometry(QRect(890, 730, 231, 41));
        toLobbyBtn = new QPushButton(RoomGame);
        toLobbyBtn->setObjectName("toLobbyBtn");
        toLobbyBtn->setGeometry(QRect(890, 670, 231, 41));
        describerText = new QPlainTextEdit(RoomGame);
        describerText->setObjectName("describerText");
        describerText->setGeometry(QRect(30, 190, 381, 461));
        describerLine = new QLineEdit(RoomGame);
        describerLine->setObjectName("describerLine");
        describerLine->setGeometry(QRect(450, 680, 371, 51));
        describerLine->setFont(font);
        describerLine->setMaxLength(128);
        guesserLine = new QLineEdit(RoomGame);
        guesserLine->setObjectName("guesserLine");
        guesserLine->setGeometry(QRect(30, 680, 381, 51));
        guesserLine->setFont(font);
        guesserLine->setMaxLength(128);
        guesserText = new QPlainTextEdit(RoomGame);
        guesserText->setObjectName("guesserText");
        guesserText->setGeometry(QRect(450, 190, 371, 461));
        refreshUserListBtn = new QPushButton(RoomGame);
        refreshUserListBtn->setObjectName("refreshUserListBtn");
        refreshUserListBtn->setGeometry(QRect(890, 190, 231, 41));
        usersList = new QListWidget(RoomGame);
        usersList->setObjectName("usersList");
        usersList->setGeometry(QRect(890, 250, 231, 401));
        wordToDescribe = new QTextEdit(RoomGame);
        wordToDescribe->setObjectName("wordToDescribe");
        wordToDescribe->setEnabled(true);
        wordToDescribe->setGeometry(QRect(0, 80, 1171, 70));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Lato")});
        font2.setPointSize(24);
        wordToDescribe->setFont(font2);
        wordToDescribe->setLineWidth(1);
        wordToDescribe->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        wordToDescribe->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        wordToDescribe->setReadOnly(true);
        msgText = new QTextEdit(RoomGame);
        msgText->setObjectName("msgText");
        msgText->setEnabled(true);
        msgText->setGeometry(QRect(340, 0, 491, 70));
        msgText->setFont(font1);
        msgText->setLineWidth(1);
        msgText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgText->setReadOnly(true);
        roundNum = new QTextEdit(RoomGame);
        roundNum->setObjectName("roundNum");
        roundNum->setEnabled(true);
        roundNum->setGeometry(QRect(0, 0, 271, 70));
        roundNum->setFont(font1);
        roundNum->setLineWidth(1);
        roundNum->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        roundNum->setReadOnly(true);

        retranslateUi(RoomGame);

        QMetaObject::connectSlotsByName(RoomGame);
    } // setupUi

    void retranslateUi(QDialog *RoomGame)
    {
        RoomGame->setWindowTitle(QCoreApplication::translate("RoomGame", "Dialog", nullptr));
        sendDescBtn->setText(QCoreApplication::translate("RoomGame", "Send description", nullptr));
        sendGuessBtn->setText(QCoreApplication::translate("RoomGame", "Send guess", nullptr));
        timeLeft->setHtml(QCoreApplication::translate("RoomGame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
        refreshUserListBtn->setText(QCoreApplication::translate("RoomGame", "Refresh ranking", nullptr));
        wordToDescribe->setHtml(QCoreApplication::translate("RoomGame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Lato'; font-size:24pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:18pt;\"><br /></p></body></html>", nullptr));
        wordToDescribe->setPlaceholderText(QString());
        msgText->setHtml(QCoreApplication::translate("RoomGame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Lato'; font-size:18pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        roundNum->setHtml(QCoreApplication::translate("RoomGame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
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
    class RoomGame: public Ui_RoomGame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROOMGAME_H
