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
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Lobby
{
public:
    QPushButton *refreshBtn;
    QListView *listView;
    QListWidget *listWidget;
    QPushButton *createRoomBtn;
    QLabel *label;

    void setupUi(QDialog *Lobby)
    {
        if (Lobby->objectName().isEmpty())
            Lobby->setObjectName("Lobby");
        Lobby->resize(806, 588);
        refreshBtn = new QPushButton(Lobby);
        refreshBtn->setObjectName("refreshBtn");
        refreshBtn->setGeometry(QRect(500, 400, 89, 25));
        listView = new QListView(Lobby);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(140, 90, 256, 192));
        listWidget = new QListWidget(Lobby);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(140, 300, 256, 192));
        createRoomBtn = new QPushButton(Lobby);
        createRoomBtn->setObjectName("createRoomBtn");
        createRoomBtn->setGeometry(QRect(500, 340, 181, 31));
        label = new QLabel(Lobby);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 30, 331, 31));

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

        createRoomBtn->setText(QCoreApplication::translate("Lobby", "Create new room", nullptr));
        label->setText(QCoreApplication::translate("Lobby", "Click on room in list to join it", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Lobby: public Ui_Lobby {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBY_H
