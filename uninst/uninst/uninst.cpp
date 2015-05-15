#include "uninst.h"

MainWidget::MainWidget(QWidget *parent)
: QDialog(parent)
{

	setFixedSize(540, 600);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	m_mainStackedLayout = new QStackedLayout();

	m_isFinish = false;
	m_onUnstTimer = NULL;
	m_onUnstThread = NULL;
	m_startUnstWidget = NULL;
	m_onUnstWidget = NULL;
	m_finishUnstWidget = NULL;

	createStartUnstWidget();
	createOnUnstWidget();
	createFinishWidget();

	m_mainStackedLayout->setCurrentWidget(m_startUnstWidget);
	//m_mainStackedLayout->setCurrentWidget(m_onUnstWidget);
	setLayout(m_mainStackedLayout);

#ifdef WINDOWS
	QString str = QStringLiteral("致云客户端卸载");
#endif
#ifdef LINUX
	QFile licence(":/txt/widowTitle"); //^^^
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString str = tc->toUnicode(ba);
#endif
	this->setFixedSize(540, 600);
	this->setWindowTitle(str); 
	this->setWindowIcon(QIcon(QString(":/system/zcloudLogo")));
	this->setWindowFlags(Qt::FramelessWindowHint);
}


MainWidget::~MainWidget()
{

}

/*三个函数界面移动*/
void MainWidget::mouseReleaseEvent(QMouseEvent *)
{
	m_mousePress = false;
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_mousePress)
	{
		QPoint move_pos = event->globalPos();
		move(move_pos - m_movePoint);
	}
}


void MainWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_mousePress = true;
	}
	m_movePoint = event->globalPos() - pos();
}

void MainWidget::doCloseMainWidgetSlot()
{
	if (!m_isFinish)
	{
#ifdef WINDOWS
		QMessageBox msgBox(QMessageBox::Question, QString(QStringLiteral("致云")), QStringLiteral("你确定要退出“致云客户端”卸载程序吗？"), QMessageBox::NoButton, this);
		msgBox.addButton(QStringLiteral("确定"), QMessageBox::AcceptRole);
		msgBox.addButton(QStringLiteral("取消"), QMessageBox::RejectRole);
#endif
#ifdef LINUX
		QFile licence;  //^^^
		licence.setFileName(":/txt/msgBoxcancel");
		licence.open(QFile::ReadOnly);
		QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
		QByteArray ba = licence.readAll();
		QString msgBoxcancel = tc->toUnicode(ba);
		licence.close();

		licence.setFileName(":/txt/msgBoxok"); //^^^
		licence.open(QFile::ReadOnly);
		ba = licence.readAll();
		QString msgBoxok = tc->toUnicode(ba);
		licence.close();

		licence.setFileName(":/txt/msgBoxquestion"); //^^^
		licence.open(QFile::ReadOnly);
		ba = licence.readAll();
		QString msgBoxquestion = tc->toUnicode(ba);
		licence.close();

		licence.setFileName(":/txt/msgBoxtitle"); //^^^
		licence.open(QFile::ReadOnly);
		ba = licence.readAll();
		QString msgBoxtitle = tc->toUnicode(ba);
		licence.close();

		QMessageBox msgBox(QMessageBox::Question, msgBoxtitle, msgBoxquestion, QMessageBox::NoButton, this);
		msgBox.addButton(msgBoxok, QMessageBox::AcceptRole);
		msgBox.addButton(msgBoxcancel, QMessageBox::RejectRole);
#endif
		msgBox.setIconPixmap(QPixmap(QString(":/system/close")));
		int ret = msgBox.exec();
		if (QMessageBox::AcceptRole == ret)//确定按钮
		{
			close();
			exit(0);
		}
		else if (QMessageBox::RejectRole == ret)//取消按钮
		{

		}
	}
	else
	{
		close();
		exit(0);
	}
}


/*下面三个是创建窗口*/
void MainWidget::createStartUnstWidget()
{
	m_startUnstWidget = new ZC_unstall();
	m_startUnstWidget->setSkinName(QString(":/img/unstallStart")); // 设置背景

	connect(m_startUnstWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_startUnstWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);

	m_startUnstBt = new PushButton(m_startUnstWidget);
	m_startUnstBt->setPicName(QString(":/img/unstallStartBt"));
	m_startUnstBt->installEventFilter(this);

	m_startUnstBt->setGeometry(162, 488, 210, 43);
	m_mainStackedLayout->addWidget(m_startUnstWidget);
}

void MainWidget::createOnUnstWidget()
{
	m_onUnstWidget = new ZC_unstall();
	m_onUnstWidget->setSkinName(QString(":/img/unstall")); // 设置背景

	connect(m_onUnstWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_onUnstWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);

	QFont ft("Cordia New");
	ft.setPixelSize(85);
	QPalette labelPalette;
	labelPalette.setBrush(QPalette::WindowText, QColor(255, 255, 255));
	m_percentLabel = new QLabel(m_onUnstWidget);
	m_percentLabel->setFont(ft);
	m_percentLabel->setPalette(labelPalette);
	m_percentLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	m_percentLabel->setText(QString("36"));

	m_percentLabel->setGeometry(243, 240, 60, 200);
	m_mainStackedLayout->addWidget(m_onUnstWidget);
}
void MainWidget::createFinishWidget()
{
	m_finishUnstWidget = new ZC_unstall();
	m_finishUnstWidget->setSkinName(QString(":/img/unstallFinish")); // 设置背景

	connect(m_finishUnstWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_finishUnstWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::eventForFinishUnshBt);

	m_finishUnstBt = new PushButton(m_finishUnstWidget);
	m_finishUnstBt->setPicName(QString(":/img/unstallFinishBt"));
	m_finishUnstBt->installEventFilter(this);

	m_onUnstTimer = NULL;

	m_finishUnstBt->setGeometry(162, 488, 210, 43);
	m_mainStackedLayout->addWidget(m_finishUnstWidget);
}

bool MainWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (m_startUnstBt == obj)
	{
		if (QEvent::MouseButtonPress == event->type())
		{
			//开始删除文件
			eventForStartUnstBt();
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (m_finishUnstBt == obj)
	{
		if (QEvent::MouseButtonPress == event->type())
		{
			//自删除之后，退出
			eventForFinishUnshBt();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void MainWidget::eventForStartUnstBt()
{
	QString path;
	QDir dir;
	path = dir.currentPath();

	m_onUnstWidget->m_closeButton->setPicName(QString(":/system/close_hover"));
	//connect(m_onUnstTimer, &QTimer::timeout, this, )
	m_onUnstThread = new UnstallThread(m_onUnstWidget);
	m_onUnstThread->setPath(path);
	connect(m_onUnstThread, &UnstallThread::deletefile, this, &MainWidget::setOnUnstallPecent);
	connect(m_onUnstThread, &UnstallThread::deleteall, this, &MainWidget::setOnUnstallFinish);
	m_onUnstThread->startJob();
	m_onUnstWidget->setdrawArc(true);
	m_mainStackedLayout->setCurrentWidget(m_onUnstWidget);
	//m_onUnstTimer->start(300);
}

void MainWidget::eventForFinishUnshBt()
{
#ifdef WINDOWS
	//自删除
	SHELLEXECUTEINFO sei;
	TCHAR szModule[MAX_PATH], szComspec[MAX_PATH], szParams[MAX_PATH];
	//获取文件路径名。 
	if ((GetModuleFileName(0, szModule, MAX_PATH) != 0) && (GetShortPathName(szModule, szModule, MAX_PATH) != 0) && (GetEnvironmentVariable(_T("COMSPEC"), szComspec, MAX_PATH) != 0))
	{
		//设置命令行参数。 
		lstrcpy(szParams, _T("/c del "));
		lstrcat(szParams, szModule);
		lstrcat(szParams, _T(" > nul"));

		//初始化SHELLEXECUTEINFO结构成员 
		sei.cbSize = sizeof(sei); //设置类型大小。
		sei.hwnd = 0; //命令窗口进程句柄，ShellExecuteEx函数执行时设置。 
		sei.lpVerb = _T("Open"); //执行动作为“打开执行”。 
		sei.lpFile = szComspec; //执行程序文件全路径名称。 
		sei.lpParameters = szParams; //执行参数。 
		sei.lpDirectory = 0;
		sei.nShow = SW_HIDE; //显示方式，此处使用隐藏方式阻止出现命令窗口界面。 
		sei.fMask = SEE_MASK_NOCLOSEPROCESS; //设置为SellExecuteEx函数结束后进程退出。 
		//创建执行命令窗口进程。 
		if (ShellExecuteEx(&sei))
		{
			//设置命令行进程的执行级别为空闲执行，这使本程序有足够的时间从内存中退出。 
			SetPriorityClass(sei.hProcess, IDLE_PRIORITY_CLASS);
			//设置本程序进程的执行级别为实时执行，这本程序马上获取CPU执行权，快速退出
			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
			//通知Windows资源浏览器，本程序文件已经被删除。
			SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, szModule, NULL);
			//执行退出程序。 
			close();
			exit(0);
		}
	}
#endif

#ifdef LINUX
	pid_t  pid;
	pid = fork();
	switch (pid) {
	case 0:
		//printf("Child process is running,CurPid is %d,ParentPid is %d\n", pid, getppid());
		usleep(300);
		remove("uninst");
		QDir dir;
		QString tmp = dir.currentPath();
		QString path = tmp.left(tmp.lastIndexOf("/zhicloud"));
		dir.setPath(dir.currentPath());
		dir.rmdir(QString("zhicloud"));
		break;
	case -1:
		//perror("Process creation failed\n");
		close();
		break;
	default:
		close();
		break;
	}

	exit(0);
#endif
}

void MainWidget::setOnUnstallPecent(int percent)
{
	m_percentLabel->setText(QString("%1").arg(percent));
	m_onUnstWidget->setPercent(percent);
#ifdef DEBUG
	std::cout << percent << "########endl";
#endif
}

void MainWidget::setOnUnstallFinish()
{
	//删除账户信息、注册表、删除快捷方式
#ifdef DEBUG
	std::cout << ".............delete all finish ";
#endif
#ifdef WINDOWS
	clearLink();
	clearRegeidt();
#endif
#ifdef LINUX
	clearConfiginiAndUserfile();
#endif
	m_mainStackedLayout->setCurrentWidget(m_finishUnstWidget);
}

#ifdef WINDOWS
bool MainWidget::clearLink()
{
	bool error = false;
	LPITEMIDLIST PIDL;
	TCHAR dir[MAX_PATH] = { 0 };
	SHGetSpecialFolderLocation(0, CSIDL_DESKTOPDIRECTORY, &PIDL);
	SHGetPathFromIDList(PIDL, dir);
	QString strDesktop = QString::fromWCharArray(dir);
	strDesktop += QStringLiteral("\\致云客户端.lnk");
	QFile::setPermissions(strDesktop, QFile::WriteOwner); //删除之前要获取写的权限
	if (!QFile::remove(strDesktop)) //........................删除桌面快捷方式
	{
		error = true;
	}
	qDebug() << "delete link............." << strDesktop;
	std::cout << ".............delete all link ";

	SHGetSpecialFolderLocation(0, CSIDL_PROGRAMS, &PIDL);
	ZeroMemory(dir, MAX_PATH);
	SHGetPathFromIDList(PIDL, dir);
	QString strStart = QString::fromWCharArray(dir);
	strStart += QStringLiteral("\\致云客户端");
	QString tmppath = strStart + QStringLiteral("\\Uninstall 致云客户端.lnk");
	QFile::setPermissions(tmppath, QFile::WriteOwner);
	if (!QFile::remove(tmppath))//.........一下两行删除开始菜单中的快捷方式
	{
		error = true;
	}
	qDebug() << "delete link............." << tmppath;

	tmppath = strStart + QStringLiteral("\\致云客户端.lnk");
	QFile::setPermissions(tmppath, QFile::WriteOwner);
	if (!QFile::remove(tmppath))
	{
		error = true;
	}
	qDebug() << "delete link............." << tmppath;

	QDir directory(strStart);
	if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))//.........删除开始中对应的菜单目录
	{
		error = true;
	}
	qDebug() << "delete link............." << tmppath;


	QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();//meybe different
	QString saveUserNamePath("");
	for (int count = 5; (saveUserNamePath.isEmpty()) && (count > 0); count++)
	{
#ifdef WINDOWS
		saveUserNamePath = environment.value(QString("LOCALAPPDATA"), QString(""));
#endif
	}
	saveUserNamePath += QString("zcconf.ini");
	QFile::setPermissions(saveUserNamePath, QFile::WriteOwner);
	if (!QFile::remove(saveUserNamePath))  //................删除账户信息
	{
		error = true;
	}
	qDebug() << "delete link............." << saveUserNamePath;

	return error;
}

bool MainWidget::RegDelnodeRecurse(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	LPTSTR lpEnd;
	LONG lResult;
	DWORD dwSize;
	TCHAR szName[MAX_PATH];
	HKEY hKey;
	FILETIME ftWrite;

	lResult = RegDeleteKeyEx(hKeyRoot, lpSubKey, KEY_WOW64_64KEY, 0);

	if (lResult == ERROR_SUCCESS)
		return true;
	lResult = RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	if (lResult != ERROR_SUCCESS)
	{
		if (lResult == ERROR_FILE_NOT_FOUND) {
			//	AfxMessageBox(_T("Key not found"));
			return true;
		}
		else {
			//AfxMessageBox(_T("Error opening key"));
			return false;
		}
	}

	lpEnd = lpSubKey + lstrlen(lpSubKey);

	if (*(lpEnd - 1) != TEXT('\\'))
	{
		*lpEnd = TEXT('\\');
		lpEnd++;
		*lpEnd = TEXT('\0');
	}

	dwSize = MAX_PATH;
	lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);

	if (lResult == ERROR_SUCCESS)
	{
		do {
			StringCchCopy(lpEnd, MAX_PATH * 2, szName);
			if (!RegDelnodeRecurse(hKeyRoot, lpSubKey)) {
				break;
			}
			dwSize = MAX_PATH;
			lResult = RegEnumKeyEx(hKey, 0, szName, &dwSize, NULL, NULL, NULL, &ftWrite);
		} while (lResult == ERROR_SUCCESS);
	}

	lpEnd--;
	*lpEnd = TEXT('\0');

	RegCloseKey(hKey);

	lResult = RegDeleteKey(hKeyRoot, lpSubKey);

	if (lResult == ERROR_SUCCESS)
		return true;

	return false;
}

bool MainWidget::RegDelnode(HKEY hKeyRoot, LPTSTR lpSubKey)
{
	TCHAR szDelKey[MAX_PATH * 2];
	StringCchCopy(szDelKey, MAX_PATH * 2, lpSubKey);
	qDebug() << "delete regedit";
	return RegDelnodeRecurse(hKeyRoot, szDelKey);
}

bool MainWidget::clearRegeidt()
{
	return RegDelnode(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\zhicloud_client"));
}
#endif

#ifdef LINUX
bool MainWidget::clearConfiginiAndUserfile()
{
	bool error = false;
	QString tmppath = QStringLiteral("/etc/zhicloud/configure.ini");
	QFile::setPermissions(tmppath, QFile::WriteOwner);
	if (!QFile::remove(tmppath))//.........删除保存安装路径和版本好的configure.ini文件
	{
		error = true;
	}
	QDir dir;
	dir.setPath(QString("/etc/"));
	if (!dir.rmdir(QString("zhicloud")))
	{
		error = true;
	}

	tmppath = QStringLiteral("/usr/share/applications/");
	QFile licence;  //^^^
	licence.setFileName(":/txt/title");
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString title = tc->toUnicode(ba);
	licence.close();
	QString fileName = title + QString(".desktop"); //桌面方式文件
	tmppath = tmppath + fileName;
	QFile::setPermissions(tmppath, QFile::WriteOwner);
	if (!QFile::remove(tmppath))//.........删除保存安装路径和版本好的configure.ini文件
	{
		error = true;
	}

	//..........删除用户账户信息

	return error;
}
#endif