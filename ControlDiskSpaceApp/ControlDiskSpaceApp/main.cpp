#include "ControlDiskSpaceApp.h"
#include <QtWidgets/QApplication>
#include <QtTest/QtTest>
#include "TestControlDiskSpace.h" 
#include <QMutex>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControlDiskSpaceApp w;
   
    w.show();
    return a.exec();
}

//QTEST_MAIN(TestControlDiskSpace)

//#include "TestControlDiskSpace.moc"