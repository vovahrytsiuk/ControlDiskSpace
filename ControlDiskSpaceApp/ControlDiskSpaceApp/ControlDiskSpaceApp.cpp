#include "ControlDiskSpaceApp.h"




ControlDiskSpaceApp::ControlDiskSpaceApp(QWidget *parent)
    : QMainWindow(parent)
{
    this->storageDevices = QStorageInfo::mountedVolumes();
    this->settingsFilePath = "settings.txt";

    fillWidgetsGrid();
    generateStartSettings();

    resize(400, 200);
    connect(this->diskFullnessSlider, &QAbstractSlider::valueChanged, this, &ControlDiskSpaceApp::updateFreeSpaceLabel);
    connect(this->StorageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControlDiskSpaceApp::updateFreeSpaceLabel);
    connect(this->saveButton, &QAbstractButton::clicked, this, &ControlDiskSpaceApp::saveSettingsChanges);
    connect(this->cancelButton, &QAbstractButton::clicked, this, &QMainWindow::hide);

    this->createTrayIcon();
    this->trayIcon->show();

    this->checker = new DiskCheckWorker(this->settingsFilePath, this->storageDevices);
    connect(checker, SIGNAL(showNotification(int)), this, SLOT(showMessage(int)));

    this->thread = new QThread(this);
 
    this->checker->moveToThread(this->thread);

    connect(this, SIGNAL(destroyed()), this, SLOT(terminateThread()));
    connect(this->thread, SIGNAL(started()), checker, SLOT(runCheck()));
    thread->start();   
}


void ControlDiskSpaceApp::showMessage(int storage_index)
{
    QString titleMessage = "Not enough free disk space " + this->storageDevices[storage_index].rootPath();
    QString bodyMessage = "Free space on " + this->storageDevices[storage_index].rootPath() + " is only " + 
        QString::number((double)this->storageDevices[storage_index].bytesFree() / 1024 / 1024 / 1024) + " GB ";
   
    this->trayIcon->showMessage(titleMessage, bodyMessage);
}



void ControlDiskSpaceApp::checkStorageDevices(const QVector<double>& reqFreeSpaceInfo)
{
    for (int i = 0; i < storageDevices.size(); i++)
    {
        double freeSpace = (double)storageDevices[i].bytesFree() / 1024 / 1024 / 1024;
        if (freeSpace < reqFreeSpaceInfo[i])
        {
           this->showMessage(i);
        }
    }
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


void ControlDiskSpaceApp::write_settings_file(const SettingsInfo& info)
{
    QFile settings_file(this->settingsFilePath);
    if (settings_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&settings_file);
        for (const auto& n : info.reqFreeSpace)
        {
            out << n << "\n";
        }
        out << info.timeout << "\n";
        settings_file.close();
    }
    else
    {
        QMessageBox::warning(this, "Error file", "Cannot open file");
    }
}

void ControlDiskSpaceApp::generateStartSettings()
{
    double freeSpaceRequirement = (100.00 - this->diskFullnessSlider->value()) / 100.00;
    QFile settingsFile(this->settingsFilePath, this);
    if (settingsFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&settingsFile);
        for (const auto& s : this->storageDevices)
        {
            double size = (double)s.bytesTotal() / 1024 / 1024 / 1024;
            out << size * freeSpaceRequirement << '\n';
        }
        out << this->timeoutSpinBox->text() << "\n";
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
    this->write_settings_file(setting_info);
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
    connect(quitAction, &QAction::triggered, this,&QCoreApplication::quit);
}




SettingsInfo DiskCheckWorker::read_file_settings()
{
    SettingsInfo info;
    QFile settings_file(this->settingFilePath);
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
    return info;
}




void DiskCheckWorker::runCheck()
{
    while (1)
    {
        auto settingsInfo = this->read_file_settings();
        this->checkStorageDevices(settingsInfo.reqFreeSpace);
        QThread::sleep(settingsInfo.timeout);

    }
}



void DiskCheckWorker::checkStorageDevices(const QVector<double>& reqFreeSpaceInfo)
{
    for (int i = 0; i < this->storageDevices.size(); i++)
    {
        double freeSpace = (double)this->storageDevices[i].bytesFree() / 1024 / 1024 / 1024;
        if (freeSpace < reqFreeSpaceInfo[i])
        {
            //run signal show message
            emit this->showNotification(i);
        }
    }
}



DiskCheckWorker::DiskCheckWorker(QString  fileSettingPath, QList<QStorageInfo> storageDevices)
{
    this->settingFilePath = fileSettingPath;
    this->storageDevices  = storageDevices;
}
