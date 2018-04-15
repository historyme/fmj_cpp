#include <QtGui/QApplication>
#include "fmjwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fmjWindow w;
    w.setWindowTitle("fmj");
    w.show();

    return a.exec();
}
