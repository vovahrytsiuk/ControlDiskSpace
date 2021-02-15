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
#include <QHBoxLayout>
#include <iostream>
#include "DiskChecker.h"


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

    QHBoxLayout* checkableLayout;
    QCheckBox* diskCheckableCheckBox;
    QLabel* diskCheckableLabel;

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

    void increment_current_position_in_widgets_grid(int& row_numbew, int& column_number, 
        int row_size, int column_size);

   
private slots:
    void showMessage(int diskPosition);
    void updateFreeSpaceLabel();
    void saveSettingsChanges();
   
};





