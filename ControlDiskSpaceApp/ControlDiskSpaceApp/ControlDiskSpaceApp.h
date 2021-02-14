#pragma once



#include <QtWidgets/QMainWindow>
#include <QStorageInfo>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCoreApplication>
#include <QThread>
#include <QDebug>
#include <QSettings>
#include <iostream>
#include "DiskChecker.h"
#include "SettingsInfo.h"











class ControlDiskSpaceApp : public QMainWindow
{

    Q_OBJECT

public:
    ControlDiskSpaceApp(QWidget *parent = Q_NULLPTR);

public slots:
    void finishChecker();
   
private:

    QList<QStorageInfo> storageDevices;  
    QGridLayout* widgetLayout;
    QGroupBox* widgetGroupBox;
    QLabel* StorageLabel;
    QComboBox* StorageComboBox;
    QLabel* diskFullnessLabel;
    QSlider* diskFullnessSlider;
    QLabel* freeSpaceLabel;
    QLabel* timeoutLabel;
    QSpinBox* timeoutSpinBox;
    QPushButton* saveButton;
    QPushButton* cancelButton;

    QString settingsFilePath;

    QSystemTrayIcon* trayIcon;

    QMenu* trayIconMenu;

    QAction* quitAction;
    QAction* showAction;
    QAction* hideAction;

    QIcon* icon;


    DiskChecker* checker;
    
    void fillWidgetsGrid();
    void fillStorageComboBox();
    void generateStartSettings();
    void createTrayIcon();
    void createActions();
    SettingsInfo read_settings_file();
   // void write_settings_file(const SettingsInfo info);
    
    
private slots:
    void showMessage(int diskPosition);
    void updateFreeSpaceLabel();
    void saveSettingsChanges();
   
};





