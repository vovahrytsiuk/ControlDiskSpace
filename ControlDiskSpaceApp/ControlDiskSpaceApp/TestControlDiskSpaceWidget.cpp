#include <QtTest/QtTest>
#include <QCoreApplication>
#include "controldiskspacewidget.h"
#include <cmath>

// add necessary includes here

class TestControlDiskSpaceWidget : public QObject
{
    Q_OBJECT

        ControlDiskSpaceWidget* controller;
    QWidget widget;
    void verifySavedSettings();
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
    void test_disk_checker_constructor();
    void test_disk_checker_start_and_finish();
    void test_disk_checker_working();

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
    //iteration on all available settigs configuration and verifying saving this information
    for (int device_i = 0; device_i < controller->StorageComboBox->count(); device_i++)//iteration by storage devices
    {
        controller->StorageComboBox->setCurrentIndex(device_i);
        qDebug() << device_i;
        for (int disk_fullness_i = controller->diskFullnessSlider->minimum();
            disk_fullness_i < controller->diskFullnessSlider->maximum(); disk_fullness_i++)//iteration on disk fullness slider
        {
            controller->diskFullnessSlider->setValue(disk_fullness_i);

            for (controller->timeoutSpinBox->setValue(controller->timeoutSpinBox->minimum());
                controller->timeoutSpinBox->value() <= controller->timeoutSpinBox->maximum();
                controller->timeoutSpinBox->stepUp())//iteration on timeout settigs(timeout spin box)
            {
                //different value of checkable current disk - true/false
                QVector<bool> checkable_states{ true, false };
                for (const auto& state : checkable_states)
                {
                    controller->diskCheckableCheckBox->setChecked(state);
                    QTest::mouseClick(controller->saveButton, Qt::LeftButton);
                    verifySavedSettings();
                }

            }
        }
    }
}



void TestControlDiskSpaceWidget::verifySavedSettings()
{
    QSettings settings(controller->settingsFilePath, QSettings::IniFormat);
    int disk_number = controller->StorageComboBox->currentIndex();
    auto disk_path = controller->StorageComboBox->currentText();
    int reqTimeout = controller->timeoutSpinBox->value();
    bool reqCheckable = controller->diskCheckableCheckBox->isChecked();

    int saved_timeout = settings.value("Time/Timeout").toInt();
    QVERIFY2(reqTimeout == saved_timeout, "Timeout value saved incorrectly");

    bool saved_checkable = settings.value(disk_path + "/Checkable").toBool();
    QVERIFY2(reqCheckable == saved_checkable, "Checkable value saved incorrectly");

    double saved_free_space_limit = settings.value(disk_path + "/Limit").toDouble();
    double req_free_space_limit = controller->storageDevices[disk_number].bytesTotal() / 1024 / 1024 / 1024;
    req_free_space_limit *= (100.00 - controller->diskFullnessSlider->value()) / 100.00;

    QVERIFY2(std::abs(req_free_space_limit - saved_free_space_limit) < 0.01, "Free space limit saved incorrectly");
}


void TestControlDiskSpaceWidget::test_disk_checker_constructor()
{
    auto devices = QStorageInfo::mountedVolumes();
    QCOMPARE(devices, controller->checker->storageDevices);

    QCOMPARE(controller->settingsFilePath, controller->checker->settingFilePath);
}

void TestControlDiskSpaceWidget::test_disk_checker_start_and_finish()
{
    QVERIFY2(!controller->checker->isRunning(), "Thread is running but should be finished");

    //emit start of checker;
    controller->startChecker();
    QVERIFY2(controller->checker->isRunning(), "Checker thread had not started");

    //emit interaption checker
    controller->finishChecker();
    QVERIFY2(!controller->checker->isRunning(), "Checker is still running while interaption has already requested");
}



void TestControlDiskSpaceWidget::test_disk_checker_working()
{
    //for each storage device we have simulate settings that set requirements free space more / less than it is right now

}

QTEST_MAIN(TestControlDiskSpaceWidget)

#include "tst_testcontroldiskspacewidget.moc"
