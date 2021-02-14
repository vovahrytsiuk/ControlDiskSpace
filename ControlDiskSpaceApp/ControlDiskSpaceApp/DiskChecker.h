#pragma once
#include <QThread>
#include <QStorageInfo>
#include <iostream>
#include <qdebug.h>
#include <qmessagebox.h>

#include "SettingsInfo.h"

#include <qsettings.h>
#include <qthread.h>


class DiskChecker : public QThread{
	Q_OBJECT
private:
	QString settingFilePath;
	QList<QStorageInfo> storageDevices;

	
public:
	DiskChecker(const QString& fileSettingPath, const QList<QStorageInfo>& storageDevices, QObject * parent = nullptr);
	void checkDisks(const QSettings& settingsInfo);
	void runChecking();
	
signals:
	void showNotification(int diskPosition);
	
protected:
	virtual void run();
};