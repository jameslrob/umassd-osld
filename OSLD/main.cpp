#include "osldisplay.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OSLDisplay w;
    w.show();

    return a.exec();
}
