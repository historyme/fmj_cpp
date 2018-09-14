#include <QtGui/QApplication>
#include "fmjwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    fmjWindow w;
    w.setWindowTitle("fmj");
    w.show();
    w.run();

    return 0;/*a.exec()*/;
}
