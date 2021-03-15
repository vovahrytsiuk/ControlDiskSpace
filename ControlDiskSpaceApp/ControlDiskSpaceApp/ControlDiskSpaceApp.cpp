#include "ControlDiskSpaceApp.h"
#include <qcoreapplication.h>



ControlDiskSpaceApp::ControlDiskSpaceApp(QWidget* parent):

	QMainWindow(parent)
{
	controllerWidget = new ControlDiskSpaceWidget(this);
	setCentralWidget(controllerWidget);
  	controllerWidget->startChecker();
	setWindowIcon(QIcon("disk-icon.png"));

	connect(controllerWidget, &ControlDiskSpaceWidget::hidingRequest, this, &QMainWindow::hide);
	connect(controllerWidget, SIGNAL(showingRequest()), this, SLOT(show()));
	connect(controllerWidget, SIGNAL(quitRequest()), this, SLOT(Quit()));
}



void ControlDiskSpaceApp::Quit()
{
	if (controllerWidget->isControlRunning())
	{
		controllerWidget->finishChecker();
	}
	QCoreApplication::quit();
}