#include "DiskChecker.h"


DiskChecker::DiskChecker(const QString& fileSettingPath, const QList<QStorageInfo>& storageDevices, QObject* parent) :
	QThread(parent)
{
	settingFilePath = fileSettingPath;
	this-> storageDevices = storageDevices;
}


void DiskChecker::checkDisks(const QSettings& settingsInfo)
{
	for (int i = 0; i < storageDevices.size(); i++)
	{
		//check if disk is in config.ini and is it checkable
		if (settingsInfo.contains(storageDevices[i].rootPath() + "/Limit") && 
			settingsInfo.value(storageDevices[i].rootPath() + "/Checkable").toBool())
		{
			double freeSpace = (double)storageDevices[i].bytesFree() / 1024 / 1024 / 1024;
			double requirements = settingsInfo.value(storageDevices[i].rootPath() 
				+ "/Limit").toDouble();
			
			if (freeSpace < requirements)
			{
				emit showNotification(i);
			}
		}
	}
}


void DiskChecker::run()
{
	forever
	{
		QSettings settings(settingFilePath, QSettings::IniFormat);
		int timeout = settings.value("Time/Timeout").toInt();
		checkDisks(settings);
		int t = 0; // time counter for sleeping; we will check is Interruption requested each 100 msec
		while (t < timeout * 1000)
		{
			if (isInterruptionRequested())
			{
				return;
			}
			t += 100;
			QThread::msleep(100);
		}

	}
	return;
}

