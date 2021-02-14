#include "ControlDiskSpaceApp.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlDiskSpaceApp w;
    w.show();
    return a.exec();
}
