#include "unzipthread.h"

UnzipThread::UnzipThread(QObject *parent)
	: QThread(parent)
{
	m_isRunning = false;
}

UnzipThread::~UnzipThread()
{

}

void UnzipThread::startJob()
{
	m_isRunning = true;
	start();
}

void UnzipThread::stopJob()
{
	m_isRunning = false;
}

void UnzipThread::run()
{
	while (m_isRunning)
	{
		

	}//end?while
}