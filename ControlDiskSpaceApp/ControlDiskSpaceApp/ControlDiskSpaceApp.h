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




