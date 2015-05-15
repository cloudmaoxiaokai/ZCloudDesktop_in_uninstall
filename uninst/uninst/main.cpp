/*中文的时候从txt文件中读的地方用^^^标记*/
#include "common.h"
#include "uninst.h"
#include "zcSingleApplication.h"
#include <QMessageBox>
#include <QFile>
#include <QtWidgets/QApplication>
#include <QTranslator>
#ifdef WINDOWS
#include <QtPlugin> 
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#pragma   comment(linker,   "/subsystem:\"windows\"   /entry:\"mainCRTStartup\""   )
#endif 

int main(int argc, char *argv[])
{
	SingleApplication a(argc, argv);
	if (a.isRunning())
	{
		QMessageBox msgBox(QMessageBox::Question, QString(QStringLiteral("致云")), QStringLiteral("下载程序已经在运行!"), QMessageBox::NoButton);
		msgBox.addButton(QStringLiteral("确定"), QMessageBox::AcceptRole);
		msgBox.exec();
		return 0;
	}

	QFile qss(":/qss/ZCInstall");
	bool b = qss.open(QFile::ReadOnly);
	qApp->setStyleSheet(qss.readAll());
	qss.close();

	QTranslator translator;
	translator.load(":/qm/qt_zh_CN");
	a.installTranslator(&translator);

	MainWidget w;
	w.show();
	return a.exec();
}
