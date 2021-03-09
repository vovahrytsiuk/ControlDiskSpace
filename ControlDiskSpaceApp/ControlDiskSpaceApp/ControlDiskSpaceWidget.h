#pragma once
#include <qwidget.h>
#include <qstring.h>
#include <qlist.h>
#include <qstorageinfo.h>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qsystemtrayicon.h>
#include <qicon.h>
#include <qmenu.h>
#include "DiskChecker.h"

class ControlDiskSpaceWidget :
    public QWidget
{
    Q_OBJECT
public:
    ControlDiskSpaceWidget(QWidget* parent = nullptr);
    bool isControlRunning();
public slots:
    void startChecker();
    void finishChecker();
    void requestHide();
    void requestShow();
    void requestQuit();
signals:
    void hidingRequest();
    void showingRequest();
    void quitRequest();
private:
    QString settingsFilePath;
    QList<QStorageInfo> storageDevices;

    //widgets
    QGridLayout* widgetLayout;
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
                    int row_size=1, int column_size=1);


private slots:
    void showMessage(int diskPosition);
    void updateFreeSpaceLabel();
    void saveSettingsChanges();
};

