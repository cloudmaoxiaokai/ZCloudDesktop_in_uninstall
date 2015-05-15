#include "zcSingleApplication.h"
#include <QString>
#include <QTextStream>
#include <QTextStream>
#include <QStringList>
#include <QAbstractSocket>
#include <QFile>

SingleApplication::SingleApplication(int argc, char **argv) :QApplication(argc, argv)
{
	_isRunning = false;
	QCoreApplication::setApplicationName("ZCUnstallServer");
	QString serverName = QCoreApplication::applicationName();

	QLocalSocket socket;
	socket.connectToServer(serverName);

	if (socket.waitForConnected(500))
	{
		QTextStream stream(&socket);
		QStringList args = QCoreApplication::arguments();

		if (args.count() > 1)
		{
			stream<<args.last();
			
		}else
		{
			stream<<QString();
		}
		stream.flush();

		socket.waitForBytesWritten();

		_isRunning = true;

		return;
	}

	server = new QLocalServer(this);
	connect(server, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));

	if (server->listen(serverName))
	{
		if (server->serverError() == QAbstractSocket::AddressInUseError && QFile::exists(server->serverName()))
		{
			QFile::remove(server->serverName());
			server->listen(serverName);
		}
	}
}

void SingleApplication ::newLocalConnection()
{
	QLocalSocket *socket = server->nextPendingConnection();
	if (!socket)
		return;
	socket->waitForReadyRead(1000);

	delete socket;
}

bool SingleApplication ::isRunning()
{
	return _isRunning;
}