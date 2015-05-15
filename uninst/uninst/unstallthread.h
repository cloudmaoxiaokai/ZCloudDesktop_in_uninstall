#ifndef UNSTALLTHREAD_H
#define UNSTALLTHREAD_H

#include "common.h"
#include "zc_unstall.h"
#include <QThread>

#include <QDir>
#include <QFileInfo>
#include <QDebug>

#include <iostream>

#ifdef WINDOWS
#include <tchar.h>
#endif

class UnstallThread : public QThread
{
	Q_OBJECT

public:
	UnstallThread(QWidget *parent=0);
	~UnstallThread();

	bool isRunning;
	void startJob();
	void stopJob();
	void setPath(const QString &unstallpath);
	/*bool removeDirWithContent(const QString &dirName);*/
signals:
	void deletefile(int percent);
	void deleteall();

protected:
	void run();
	bool deleteDir(const QString &dirName);
	QString    m_unstallPath;
};

#endif // UNSTALLTHREAD_H
