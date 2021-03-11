#include <QtTest/QtTest>
#include <QCoreApplication>
#include "controldiskspacewidget.h"

// add necessary includes here

class TestControlDiskSpaceWidget : public QObject
{
    Q_OBJECT

        ControlDiskSpaceWidget* controller;
    QWidget widget;
public:
    TestControlDiskSpaceWidget();
    ~TestControlDiskSpaceWidget();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();
    void test_storage_combo_box();
    void test_hide_button();
    void test_tray_actions();
    void test_save_button();

};

TestControlDiskSpaceWidget::TestControlDiskSpaceWidget()
{

}

TestControlDiskSpaceWidget::~TestControlDiskSpaceWidget()
{

}

void TestControlDiskSpaceWidget::initTestCase()
{
    controller = new ControlDiskSpaceWidget(&widget);
}

void TestControlDiskSpaceWidget::cleanupTestCase()
{
    delete controller;
}

void TestControlDiskSpaceWidget::test_case1()
{

}

void TestControlDiskSpaceWidget::test_storage_combo_box()
{
    auto storage_devices = QStorageInfo::mountedVolumes();
    int device_count = storage_devices.size();
    for (int i = 0; i < device_count; i++)
    {
        qDebug() << controller->StorageComboBox->currentData();
        QCOMPARE(controller->StorageComboBox->itemText(i), storage_devices[i].rootPath());
    }
}

void TestControlDiskSpaceWidget::test_hide_button()
{
    QSignalSpy spy(controller, SIGNAL(hidingRequest()));//for catching hidingRequest signal

    //triggered signal
    QTest::mouseClick(controller->cancelButton, Qt::LeftButton);

    // make sure the signal was emitted exactly one time
    QCOMPARE(spy.count(), 1);

}

void TestControlDiskSpaceWidget::test_tray_actions()
{
    QSignalSpy hide_spy(controller, SIGNAL(hidingRequest()));
    controller->hideAction->trigger();
    QCOMPARE(hide_spy.count(), 1);

    QSignalSpy show_spy(controller, SIGNAL(showingRequest()));
    controller->showAction->trigger();
    QCOMPARE(show_spy.count(), 1);

    QSignalSpy quit_spy(controller, SIGNAL(quitRequest()));
    controller->quitAction->trigger();
    QCOMPARE(quit_spy.count(), 1);
}

void TestControlDiskSpaceWidget::test_save_button()
{

}

QTEST_MAIN(TestControlDiskSpaceWidget)

#include "tst_testcontroldiskspacewidget.moc"
