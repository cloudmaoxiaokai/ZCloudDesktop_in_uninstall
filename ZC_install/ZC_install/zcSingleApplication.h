#ifndef ZC_SINGLE_APPLICATION_H
#define ZC_SINGLE_APPLICATION_H

#include <QApplication>
#include <QCoreApplication>
#include <QLocalSocket>
#include <QLocalServer>

class MainWindow;

class SingleApplication :public QApplication
{
	Q_OBJECT
public:
	SingleApplication(int argc, char **argv);

	bool isRunning();

private slots:
	void newLocalConnection();

private:
	QLocalServer *server;

	bool _isRunning;

};
#endif