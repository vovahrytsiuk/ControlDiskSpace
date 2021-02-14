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
   

    //checker->runChecking();
    checker->start();

       


    /*
    connect(this, SIGNAL(destroyed()), this, SLOT(terminateThread()));
 */
}


void ControlDiskSpaceApp::showMessage(int diskPosition)
{
    QString titleMessage = "Not enough free disk space " + storageDevices[diskPosition].rootPath();
    QString bodyMessage = "Free space on " + storageDevices[diskPosition].rootPath() + " is only " + 
        QString::number((double)storageDevices[diskPosition].bytesFree() / 1024 / 1024 / 1024) + " GB ";
   
    this->trayIcon->showMessage(titleMessage, bodyMessage);
}







SettingsInfo ControlDiskSpaceApp::read_settings_file()
{
    SettingsInfo info;
    QFile settings_file(this->settingsFilePath);
    if (settings_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&settings_file);
        int device_count = this->storageDevices.size();
        for (int i = 0; i < device_count; i++)
        {
            double tmp;
            in >> tmp;
            info.reqFreeSpace.push_back(tmp);
        }
        in >> info.timeout;
        settings_file.close();
    }
    else
    {
        QMessageBox::warning(this, "Error file", "Cannot open file");
    }
    return info;
}


//void ControlDiskSpaceApp::write_settings_file(const SettingsInfo& info)
//{
//    QFile settings_file(this->settingsFilePath);
//    if (settings_file.open(QIODevice::WriteOnly | QIODevice::Text))
//    {
//        QTextStream out(&settings_file);
//        for (const auto& n : info.reqFreeSpace)
//        {
//            out << n << "\n";
//        }
//        out << info.timeout << "\n";
//        settings_file.close();
//    }
//    else
//    {
//        QMessageBox::warning(this, "Error file", "Cannot open file");
//    }
//}

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
        this->StorageComboBox->addItem(s.rootPath());
    }
}


void ControlDiskSpaceApp::fillWidgetsGrid()
{
    this->widgetLayout = new QGridLayout(this);
    

    this->StorageLabel = new QLabel(this);
    this->StorageLabel->setText("Storage device");
    this->widgetLayout->addWidget(this->StorageLabel, 0, 0);

    this->StorageComboBox = new QComboBox(this);
    this->fillStorageComboBox();
    this->widgetLayout->addWidget(this->StorageComboBox, 0, 1);

    this->diskFullnessLabel = new QLabel(this);
    this->diskFullnessLabel->setText("Permissible disk fullness");
    this->widgetLayout->addWidget(this->diskFullnessLabel, 1, 0);

    this->diskFullnessSlider = new QSlider(Qt::Horizontal, this);
    this->diskFullnessSlider->setSliderPosition(90);
    this->widgetLayout->addWidget(this->diskFullnessSlider, 1, 1);

    this->freeSpaceLabel = new QLabel(this);
    this->updateFreeSpaceLabel();
    this->widgetLayout->addWidget(this->freeSpaceLabel, 2, 0, 1, 2);

    this->timeoutLabel = new QLabel(this);
    this->timeoutLabel->setText("Timeout");
    this->widgetLayout->addWidget(this->timeoutLabel, 3, 0);

    this->timeoutSpinBox = new QSpinBox(this);
    this->timeoutSpinBox->setMinimum(5);
    this->timeoutSpinBox->setMaximum(10000);
    this->timeoutSpinBox->setValue(10);
    this->widgetLayout->addWidget(this->timeoutSpinBox, 3, 1);

    this->cancelButton = new QPushButton("Hide", this);
    this->widgetLayout->addWidget(this->cancelButton, 4, 0);

    this->saveButton = new QPushButton("Save", this);
    this->widgetLayout->addWidget(this->saveButton, 4, 1);

    this->widgetGroupBox = new QGroupBox(this);
    this->widgetGroupBox->setLayout(this->widgetLayout);

    this->setCentralWidget(this->widgetGroupBox);
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
    auto setting_info = this->read_settings_file();
    int index_changed = this->StorageComboBox->currentIndex();
    double newReqFreeSpace = (double)this->storageDevices[this->StorageComboBox->currentIndex()].bytesTotal() / 1024 / 1024 / 1024;
    newReqFreeSpace *= (100.00 - this->diskFullnessSlider->value()) / 100.00;
    setting_info.reqFreeSpace[index_changed] = newReqFreeSpace;
    setting_info.timeout = this->timeoutSpinBox->text().toInt();
   // this->write_settings_file(setting_info);
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




