#include "gui.h"

#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("Lato");
    QApplication::setFont(font);
    Gui w;
    w.show();
    return a.exec();
}
