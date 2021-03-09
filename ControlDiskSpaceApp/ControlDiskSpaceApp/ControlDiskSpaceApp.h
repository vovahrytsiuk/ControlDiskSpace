#pragma once



#include <qmainwindow.h>
#include "ControlDiskSpaceWidget.h"




class ControlDiskSpaceApp : public QMainWindow
{
	Q_OBJECT
public:
	ControlDiskSpaceApp(QWidget* parent = nullptr);
private:
	ControlDiskSpaceWidget* controllerWidget;
private slots:
	void Quit();
};


//class ControlDiskSpaceApp : public QMainWindow
//{
//
//    Q_OBJECT
//
//public:
//    friend class TestControlDiskSpace;
//    ControlDiskSpaceApp(QWidget *parent = Q_NULLPTR);
//    QString getSettingsFilePath();
//    QList<QStorageInfo> getStorageDevicesList();
//
//public slots:
//    void finishChecker();
//   
//private:
//
//    QList<QStorageInfo> storageDevices;  
//    QGridLayout* widgetLayout;
//    QGroupBox* widgetGroupBox;
//    QLabel* StorageLabel;
//    QComboBox* StorageComboBox;
//    QLabel* diskFullnessLabel;
//    QSlider* diskFullnessSlider;
//    QLabel* freeSpaceLabel;
//    QLabel* timeoutLabel;
//    QSpinBox* timeoutSpinBox;
//    QPushButton* saveButton;
//    QPushButton* cancelButton;
//
//    QHBoxLayout* checkableLayout;
//    QCheckBox* diskCheckableCheckBox;
//    QLabel* diskCheckableLabel;
//
//    QString settingsFilePath;
//
//    QSystemTrayIcon* trayIcon;
//
//    QMenu* trayIconMenu;
//
//    QAction* quitAction;
//    QAction* showAction;
//    QAction* hideAction;
//
//    QIcon* icon;
//
//    DiskChecker* checker;
//    
//    void fillWidgetsGrid();
//    void fillStorageComboBox();
//    void generateStartSettings();
//    void createTrayIcon();
//    void createActions();
//
//    void increment_current_position_in_widgets_grid(int& row_numbew, int& column_number, 
//        int row_size, int column_size);
//
//   
//private slots:
//    void showMessage(int diskPosition);
//    void updateFreeSpaceLabel();
//    void saveSettingsChanges();
//   
//};





