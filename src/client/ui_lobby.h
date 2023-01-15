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
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Lobby
{
public:
    QPushButton *refreshBtn;
    QListView *listView;
    QListWidget *listWidget;
    QPushButton *createRoomBtn;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *roomName;
    QTextEdit *msgText;

    void setupUi(QDialog *Lobby)
    {
        if (Lobby->objectName().isEmpty())
            Lobby->setObjectName("Lobby");
        Lobby->resize(843, 681);
        QFont font;
        font.setFamilies({QString::fromUtf8("Lato")});
        font.setPointSize(16);
        Lobby->setFont(font);
        refreshBtn = new QPushButton(Lobby);
        refreshBtn->setObjectName("refreshBtn");
        refreshBtn->setGeometry(QRect(550, 140, 231, 41));
        listView = new QListView(Lobby);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(80, 220, 256, 192));
        listWidget = new QListWidget(Lobby);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(90, 440, 256, 192));
        createRoomBtn = new QPushButton(Lobby);
        createRoomBtn->setObjectName("createRoomBtn");
        createRoomBtn->setGeometry(QRect(550, 420, 231, 41));
        label = new QLabel(Lobby);
        label->setObjectName("label");
        label->setGeometry(QRect(60, 160, 331, 31));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(Lobby);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(550, 270, 231, 31));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(Lobby);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(60, 110, 201, 31));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lato")});
        font1.setPointSize(24);
        label_3->setFont(font1);
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4 = new QLabel(Lobby);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(550, 320, 231, 31));
        label_4->setAlignment(Qt::AlignCenter);
        roomName = new QLineEdit(Lobby);
        roomName->setObjectName("roomName");
        roomName->setGeometry(QRect(550, 360, 231, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Lato")});
        font2.setPointSize(14);
        roomName->setFont(font2);
        roomName->setMaxLength(20);
        roomName->setCursorPosition(0);
        msgText = new QTextEdit(Lobby);
        msgText->setObjectName("msgText");
        msgText->setEnabled(true);
        msgText->setGeometry(QRect(0, 20, 851, 70));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Lato")});
        font3.setPointSize(18);
        msgText->setFont(font3);
        msgText->setLineWidth(1);
        msgText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgText->setReadOnly(true);

        retranslateUi(Lobby);

        QMetaObject::connectSlotsByName(Lobby);
    } // setupUi

    void retranslateUi(QDialog *Lobby)
    {
        Lobby->setWindowTitle(QCoreApplication::translate("Lobby", "Dialog", nullptr));
        refreshBtn->setText(QCoreApplication::translate("Lobby", "Refresh list", nullptr));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("Lobby", "dsa", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("Lobby", "dsa", nullptr));
        listWidget->setSortingEnabled(__sortingEnabled);

        createRoomBtn->setText(QCoreApplication::translate("Lobby", "Create a new room", nullptr));
        label->setText(QCoreApplication::translate("Lobby", "Click on room in list to join it", nullptr));
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
    } // retranslateUi

};

namespace Ui {
    class Lobby: public Ui_Lobby {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBY_H
