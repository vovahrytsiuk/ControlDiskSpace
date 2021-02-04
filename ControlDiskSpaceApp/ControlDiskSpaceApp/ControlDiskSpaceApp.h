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
#include <iostream>

struct SettingsInfo {
    QVector<double> reqFreeSpace;
    int timeout;
};




class ControlDiskSpaceApp : public QMainWindow
{

    Q_OBJECT

public:
    ControlDiskSpaceApp(QWidget *parent = Q_NULLPTR);
   

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

    QThread* thread;
    
    void fillWidgetsGrid();
    void fillStorageComboBox();
    void generateStartSettings();
    void createTrayIcon();
    void createActions();
    SettingsInfo read_settings_file();
    void write_settings_file(const SettingsInfo& info);
    void checkStorageDevices(const QVector<double>& reqFreeSpaceInfo);
    
     

private slots:
    void showMessage(int storage_index);
    void runControl();
    void updateFreeSpaceLabel();
    void saveSettingsChanges();
    
    //void iconActivated(QSystemTrayIcon::ActivationReason reason);
    
    
};





