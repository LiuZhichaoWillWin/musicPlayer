#include "Control.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Control w;
    w.show();
    return a.exec();
}
