#include "ControlDiskSpaceApp.h"




ControlDiskSpaceApp::ControlDiskSpaceApp(QWidget* parent)
    : QMainWindow(parent)
{
    std::cout << "hello" << std::endl;
    storageDevices = QStorageInfo::mountedVolumes();
    settingsFilePath = "config.ini";

    fillWidgetsGrid();
    generateStartSettings();

    resize(400, 200);
    connect(this->diskFullnessSlider, &QAbstractSlider::valueChanged, this,
        &ControlDiskSpaceApp::updateFreeSpaceLabel);
    connect(this->StorageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &ControlDiskSpaceApp::updateFreeSpaceLabel);
    connect(this->saveButton, &QAbstractButton::clicked, this,
        &ControlDiskSpaceApp::saveSettingsChanges);
    connect(this->cancelButton, &QAbstractButton::clicked, this, &QMainWindow::hide);
    
    createTrayIcon();
    trayIcon->show();

    checker = new DiskChecker(settingsFilePath, storageDevices, this);
    connect(checker, SIGNAL(showNotification(int)), this, 
        SLOT(showMessage(int)));
    connect(checker, SIGNAL(finished()), this, SLOT(showMessageBox()));
  
    qDebug() << "Start checker";
    checker->start();

}


void ControlDiskSpaceApp::showMessage(int diskPosition)
{
    QString titleMessage = "Not enough free disk space " + storageDevices[diskPosition].rootPath();
    QString bodyMessage = "Free space on " + storageDevices[diskPosition].rootPath() + " is only " + 
        QString::number((double)storageDevices[diskPosition].bytesFree() / 1024 / 1024 / 1024) + " GB ";
   
    trayIcon->showMessage(titleMessage, bodyMessage);
}

void ControlDiskSpaceApp::generateStartSettings()
{
    double freeSpaceRequirement = (100.00 - this->diskFullnessSlider->value()) / 100.00;
    QSettings settings(settingsFilePath, QSettings::IniFormat);
    settings.setValue("Time/Timeout", 10);
    for (const auto& d : storageDevices)
    {
        settings.setValue(d.rootPath() + "/Checkable", true);
        double size = (double)d.bytesTotal() / 1024 / 1024 / 1024;
        settings.setValue(d.rootPath() + "/Limit", size * freeSpaceRequirement);
    }
}

void ControlDiskSpaceApp::fillStorageComboBox()
{
    for (const auto& s : this->storageDevices)
    {
        StorageComboBox->addItem(s.rootPath());
    }
}


void ControlDiskSpaceApp::increment_current_position_in_widgets_grid(int& row_number, 
    int& column_number,  int row_size = 1, int column_size = 1) {
    row_number += column_number + row_size - 1 + column_size - 1; 
    column_number += column_size;
    column_number %= 2;
}


void ControlDiskSpaceApp::fillWidgetsGrid()
{
    int row_number = 0;
    int column_number = 0; //row_number & column_number are value that define coordinates of widget position
                            //in grid layout for central widget
                            // column number in range 0 .. 1
                            // row_number in range 0..?
                            //to go to next position in grid use method
                            //increment_current_position_in_widgets_grid(row_number, column_number);
    widgetLayout = new QGridLayout(this);
    

    StorageLabel = new QLabel(this);
    StorageLabel->setText("Storage device");
    widgetLayout->addWidget(StorageLabel, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    StorageComboBox = new QComboBox(this);
    fillStorageComboBox();

    diskCheckableLabel = new QLabel(this);
    diskCheckableLabel->setText("Checkable");

    diskCheckableCheckBox = new QCheckBox(this);

    checkableLayout = new QHBoxLayout(this);
    checkableLayout->addWidget(StorageComboBox);
    checkableLayout->addWidget(diskCheckableLabel);
    checkableLayout->addWidget(diskCheckableCheckBox);
    widgetLayout->addLayout(checkableLayout, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    diskFullnessLabel = new QLabel(this);
    diskFullnessLabel->setText("Permissible disk fullness");
    widgetLayout->addWidget(diskFullnessLabel, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    diskFullnessSlider = new QSlider(Qt::Horizontal, this);
    diskFullnessSlider->setSliderPosition(90);
    widgetLayout->addWidget(diskFullnessSlider, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    freeSpaceLabel = new QLabel(this);
    updateFreeSpaceLabel();
    widgetLayout->addWidget(this->freeSpaceLabel, row_number, column_number, 1, 2);
    increment_current_position_in_widgets_grid(row_number, column_number, 1, 2);

    timeoutLabel = new QLabel(this);
    timeoutLabel->setText("Timeout");
    widgetLayout->addWidget(this->timeoutLabel, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    timeoutSpinBox = new QSpinBox(this);
    timeoutSpinBox->setMinimum(5);
    timeoutSpinBox->setMaximum(10000);
    timeoutSpinBox->setValue(10);
    widgetLayout->addWidget(timeoutSpinBox, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    cancelButton = new QPushButton("Hide", this);
    widgetLayout->addWidget(cancelButton, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    saveButton = new QPushButton("Save", this);
    widgetLayout->addWidget(this->saveButton, row_number, column_number);
    increment_current_position_in_widgets_grid(row_number, column_number);

    widgetGroupBox = new QGroupBox(this);
    widgetGroupBox->setLayout(this->widgetLayout);

    setCentralWidget(this->widgetGroupBox);
}

void ControlDiskSpaceApp::updateFreeSpaceLabel()
{
    QString str = "Minimal free space on disk ";
    double memory_size = (double)this->storageDevices[this->StorageComboBox->currentIndex()].bytesTotal() / 1024 / 1024 / 1024;
    double freeSpaceRequirement = (100.00 - this->diskFullnessSlider->value()) / 100.00;
    str += QString::number(memory_size * freeSpaceRequirement);
    str += " GB ";
    this->freeSpaceLabel->setText(str);
}


void ControlDiskSpaceApp::saveSettingsChanges()
{
    int new_timeout = timeoutSpinBox->value();
    QStorageInfo disk = storageDevices[StorageComboBox->currentIndex()];
    bool checkable = diskCheckableCheckBox->isChecked();

    double freeSpaceRequirement = (100.00 - this->diskFullnessSlider->value()) / 100.00;
    freeSpaceRequirement *= disk.bytesTotal() / 1024 / 1024 / 1024;

    QSettings settings(settingsFilePath, QSettings::IniFormat);
    settings.setValue(disk.rootPath() + "/Limit", freeSpaceRequirement);
    settings.setValue(disk.rootPath() + "/Checkable", checkable);
    settings.setValue("Time/Timeout", new_timeout);
}


void ControlDiskSpaceApp::createTrayIcon()
{
    this->icon = new QIcon("disk-icon.png");

    this->trayIcon = new QSystemTrayIcon(this);
    this->createActions();
    this->trayIcon->setIcon(*icon);
    setWindowIcon(*icon);
    this->trayIcon->setToolTip("DiskSpaceController");

    this->trayIconMenu = new QMenu(this);
    this->trayIconMenu->addAction(this->showAction);
    this->trayIconMenu->addAction(this->hideAction);
    this->trayIconMenu->addSeparator();
    this->trayIconMenu->addAction(quitAction);
    this->trayIcon->setContextMenu(this->trayIconMenu);
}


void ControlDiskSpaceApp::createActions()
{
    this->showAction = new QAction("Show gui", this);
    connect(showAction, &QAction::triggered, this, &QMainWindow::show);

    this->hideAction = new QAction("Hide gui", this);
    connect(hideAction, &QAction::triggered, this, &QMainWindow::hide);

    this->quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, this, &ControlDiskSpaceApp::finishChecker);
}


void ControlDiskSpaceApp::finishChecker()
{
    qDebug() << "interaption requested";
    checker->requestInterruption();
    qDebug() << "startwaiting";
    checker->wait();
    qDebug() << "FinishWaiting";
    QCoreApplication::quit();
}




