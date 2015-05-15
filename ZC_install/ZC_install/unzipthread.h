#ifndef UNZIPTHREAD_H
#define UNZIPTHREAD_H

#include "common.h"
#include <QThread>



class UnzipThread : public QThread
{
	Q_OBJECT

public:
	UnzipThread(QObject *parent=0);
	~UnzipThread();

	volatile bool m_isRunning;
	void stopJob();
	void startJob();

signals:
	void unzipPercent(int percent);//增长一个解压百分比就发送一次信号
	void unzipComplete(); //解压完成时发出信号

private:
	void run();
};

#endif // UNZIPTHREAD_H
