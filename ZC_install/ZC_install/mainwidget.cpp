#include "mainwidget.h"
#include <QDebug>

extern QString g_zcSysFontFamily;

MainWidget::MainWidget(QWidget *parent)
	: QDialog(parent)
{ 
	m_isFinish = false;
	setFixedSize(540, 600);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	m_mainStackedLayout = new QStackedLayout();
	
	createSetPathWidget();
	createUpdateWidget();
	createLicenceWidget();
	createCopyWidget();
	createFinishCopyWidget();

	//检查是否已经安装
	if (1 == isInstallClient()) //返回1，已经安装，显示更新界面
	{
		m_mainStackedLayout->setCurrentWidget(m_updateWidget);
	}
	else if (0 == isInstallClient())
	{
		m_mainStackedLayout->setCurrentWidget(m_setPathParentWid);
	}

	setLayout(m_mainStackedLayout);

#ifdef WINDOWS
	QString str = QStringLiteral("致云科技云主机终端");
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

void MainWidget::showWidget(QWidget *nextWidget)
{
	m_mainStackedLayout->setCurrentWidget(nextWidget);
}

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

void MainWidget::createSetPathWidget()
{
	QFont ft(g_zcSysFontFamily);
	ft.setPixelSize(12);
	QPalette labelPalette;
	labelPalette.setColor(QPalette::WindowText, QColor(17, 142, 188));

	m_setPathParentWid = new QWidget();//窗口的父类
	m_setPathParentWid->setFixedSize(540, 600);
	m_setPathParentWid->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
//start ? m_setPathUserDefWid
	m_setPathUserDefWid = new QWidget();//选择路径的widget
	m_setPathUserDefWid->setFixedSize(540, 53);
	m_setPathUserDefWid->setObjectName(QString("userDefWid"));
	m_setPathUserDefWid->setAutoFillBackground(true);
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap(":/beforeCopy/userDefPathBg")));
	m_setPathUserDefWid->setPalette(palette);
	m_setPathUserDefWid->hide();

	m_userSelectPath = new PushButton(m_setPathUserDefWid);
	m_userSelectPath->setPicName(QString(":/beforeCopy/selectUserDefButton"));
	connect(m_userSelectPath, &PushButton::clicked, this, &MainWidget::doSelectUserDefPathSlot);

	//m_userSelectPath->setGeometry()
	m_userDefPath = new QLineEdit(m_setPathUserDefWid);
	m_userDefPath->setStyleSheet("QLineEdit#userDefPath{background-color:transparent;border:0;}");
	labelPalette.setColor(QPalette::Text, QColor(255, 255, 255));
	m_userDefPath->setPalette(labelPalette);
	ft.setFamily(g_zcSysFontFamily);
	ft.setPixelSize(12);
	m_userDefPath->setFont(ft);
	m_userDefPath->setObjectName(QString("userDefPath"));

	QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();//meybe different
	QString defaultPath("");
	for (int count = 5; (defaultPath.isEmpty()) && (count > 0); count++)
	{
#ifdef WINDOWS
		defaultPath = environment.value(QString("ProgramFiles(x86)"), QString(""));
#endif
#ifdef LINUX
		defaultPath = QString("/usr/share");
#endif
	}
	m_userDefPath->setText(defaultPath);

	m_userSelectPath->setGeometry(475, 18, 12, 12);
	m_userDefPath->setGeometry(113, 18, 360, 12);
//end ? m_setPathUserDefWid

//start ? m_setPathWidget
	m_setPathWidget = new ZC_install();
	m_setPathWidget->setAutoFillBackground(true);
	m_setPathWidget->setSkinName(QString(":/beforeCopy/background"));
	connect(m_setPathWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_setPathWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);
// 	QPalette palette;
// 	palette.setBrush(QPalette::Background, QBrush(QPixmap(":/beforeCopy/background")));
// 	m_setPathWidget->setPalette(palette);

	m_setPathInstallButton = new PushButton(m_setPathWidget);
	m_setPathInstallButton->setPicName(":/beforeCopy/installButton");
	connect(m_setPathInstallButton, &PushButton::clicked, this, &MainWidget::doShowNextWidgetSlot);

	m_protocolLabel = new MyClickLabel(m_setPathWidget);
	m_protocolLabel->setCursor(QCursor(Qt::PointingHandCursor));		//设置QLabel控件的鼠标形状
	ft.setFamily(g_zcSysFontFamily);
	ft.setPixelSize(12);
	m_protocolLabel->setFont(ft);
	m_protocolLabel->setPalette(labelPalette);
#ifdef WINDOWS
	m_protocolLabel->setText(QStringLiteral("《用户许可协议》"));
#endif
#ifdef LINUX
	QFile licence(":/txt/userLicense");  //^^^
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString str = tc->toUnicode(ba);
	m_protocolLabel->setText(str);
	licence.close();
#endif
	connect(m_protocolLabel, &MyClickLabel::clicked, this, &MainWidget::doShowProtocolContentSlot);
	m_agreeLebel = new QLabel(m_setPathWidget);
	labelPalette.setColor(QPalette::WindowText, QColor(178, 249, 251));
	m_agreeLebel->setPalette(labelPalette);
	ft.setFamily(g_zcSysFontFamily);
	ft.setPixelSize(12);
	m_agreeLebel->setFont(ft);
#ifdef WINDOWS
	m_agreeLebel->setText(QStringLiteral("同意"));
#endif
#ifdef LINUX
	licence.setFileName(":/txt/agreeLabel"); //^^^
	licence.open(QFile::ReadOnly);
	ba = licence.readAll();
	str = tc->toUnicode(ba);
	m_agreeLebel->setText(str);
	licence.close();
#endif
	m_agreeButton = new CheckButton(m_setPathWidget);
	m_agreeButton->setPicName(":/beforeCopy/check");
	m_agreeButton->update();
	connect(m_agreeButton, &CheckButton::clicked, this, &MainWidget::doSwitchAgreeButtonSlot);

	m_userDefLabel = new QLabel(m_setPathWidget);
	labelPalette.setColor(QPalette::WindowText, QColor(178, 249, 251));
	m_userDefLabel->setPalette(labelPalette);
	ft.setFamily(g_zcSysFontFamily);
	ft.setPixelSize(12);
	m_userDefLabel->setFont(ft);
#ifdef WINDOWS
	m_userDefLabel->setText(QStringLiteral("自定义"));
#endif
#ifdef LINUX
	licence.setFileName(":/txt/userDefLabel"); //^^^
	licence.open(QFile::ReadOnly);
	ba = licence.readAll();
	str = tc->toUnicode(ba);
	m_userDefLabel->setText(str);
	licence.close();
#endif
	m_userDefLabel->setCursor(QCursor(Qt::PointingHandCursor));
	m_userDefButton = new CheckButton(m_setPathWidget);
	m_userDefButton->setCursor(QCursor(Qt::PointingHandCursor));
	m_userDefButton->setPicName(QString(":/beforeCopy/userDefButton"));
	m_userDefButton->update();
	connect(m_userDefButton, &CheckButton::clicked, this, &MainWidget::doExtendSetPathWidgetSlot);

	m_agreeButton->setGeometry(QRect(10, 578, 13, 12));
	m_agreeLebel->setGeometry(41, 578, 60, 12);
	m_protocolLabel->setGeometry(67, 578, 200, 12);
	m_userDefLabel->setGeometry(480, 578, 100, 12);
	m_userDefButton->setGeometry(519, 579, 11, 11);
	m_setPathInstallButton->setGeometry(162, 488, 208, 40);
//end ? m_setPathWidget
	m_setPathLayout = new QVBoxLayout();
	m_setPathLayout->setContentsMargins(0, 0, 0, 0);
	m_setPathLayout->addWidget(m_setPathWidget);
	m_setPathLayout->addWidget(m_setPathUserDefWid);
	m_setPathLayout->setSizeConstraint(QLayout::SetFixedSize);//保证对话框的尺寸相对固定始终是各控件组合的尺寸
	m_setPathParentWid->setLayout(m_setPathLayout);

	m_mainStackedLayout->addWidget(m_setPathParentWid);
	//http://www.cnblogs.com/rollenholt/archive/2012/04/10/2441249.html
}

void MainWidget::createLicenceWidget()
{
	QFont ft(g_zcSysFontFamily);
	ft.setPixelSize(12);
	QPalette labelPalette;
	labelPalette.setColor(QPalette::WindowText, QColor(30, 30, 30, 255));

	m_licenceWidget = new ZC_install();
	m_licenceWidget->setSkinName(QString(":/beforeCopy/licenceBackground"));
	connect(m_licenceWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_licenceWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);
	m_licenceWidget->setAttribute(Qt::WA_TranslucentBackground);
// 	QPainterPath path;//圆角设置
// 	QRectF rect = QRectF(24, 81, 489, 360);
// 	path.addRoundedRect(rect, 40, 40);
// 	QPolygon polygon = path.toFillPolygon().toPolygon();
// 	QRegion region(polygon);
	m_licenceLabel = new QLabel();
	m_licenceLabel->setFont(ft);
	m_licenceLabel->setPalette(labelPalette);
	m_licenceLabel->setObjectName("licenceLabel");
	m_licenceLabel->setStyleSheet("QLabel#licenceLabel{background-color:white;}");
	m_licenceLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//m_licenceLabel->setMask(region);
	QFile licence(":/beforeCopy/licenceTxt");
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString str = tc->toUnicode(ba);
	m_licenceLabel->setText(str);
	licence.close();

	m_licenceButton = new PushButton(m_licenceWidget);
	m_licenceButton->setPicName(QString(":/beforeCopy/licenceButton"));
	m_licenceButton->installEventFilter(this);

	m_licenceScroll = new QScrollArea(m_licenceWidget);
	//m_licenceScroll->setBackgroundRole();
	m_licenceScroll->setWidget(m_licenceLabel);
	m_licenceScroll->setStyleSheet(
		//"QScrollBar:vertical{width:18px;background:rgba(0, 0, 0, 0% );margin:0px, 0px, 0px, 0px;padding-top:0px; padding-bottom:0px;}"
		"QScrollBar::handle:vertical{width:18px;background:rgba(0, 147, 255, 100% );border-radius:4px; min-height:60px;margin: 0px 1px 0px 0px;}"
		"QScrollBar::handle:vertical:hover{width:18px;background:rgba(0, 147, 255, 80% );border-radius:4px;min-height:60px;}"
		"QScrollBar::handle:vertical:pressed{width:18px;background:rgba(0, 0, 255, 80% );border-radius:4px;min-height:60px;}"
		"QScrollBar::add-line:vertical{height:0px; width:0px;subcontrol-position:bottom;}"
		"QScrollBar::sub-line:vertical{height:0px; width:0px;subcontrol-position:top;}"
		"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:rgb(239, 239, 239);border-radius:4px;}"
		);
	m_licenceScroll->setAutoFillBackground(true);
// 	m_licenceScroll->setStyleSheet(\
// 		"QScrollBar:vertical{width:40px;background:rgba(0, 0, 0, 50%);}"
// 		);
	m_licenceScroll->verticalScrollBar()->setStyleSheet(
		"QScrollBar::vertical {background-color: rgb(239, 239, 239);width: 18px;}"
		);
	m_licenceScroll->setBackgroundRole(QPalette::Light);
	m_licenceScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	m_licenceLabel->setFixedSize(489, 30000);
	m_licenceButton->setGeometry(162, 488, 210, 41);
	m_licenceScroll->setGeometry(24, 81, 489, 360);

	m_mainStackedLayout->addWidget(m_licenceWidget);
}

void MainWidget::createCopyWidget()
{
	QFont ft(g_zcSysFontFamily);
	ft.setPixelSize(58);
	QPalette labelPalette;
	labelPalette.setColor(QPalette::WindowText, QColor(255, 255, 255));
	//labelPalette.setBrush(QPalette::WindowText, QBrush(QPixmap(100, 100)));

	m_copyWidget = new ZC_install();
	m_copyWidget->setSkinName(QString(":/copy/background"));
	connect(m_copyWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_copyWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);
	m_percentLabel = new QLabel(m_copyWidget);
//	ft.setWeight(1);   //设置字体粗细，但是效果不好
	//ft1.setBold(true);
//	ft1.setStretch(1);
//	m_percentLabel->setFont(ft);
	//QFont ft1("Code Light");
	QFont ft1("Cordia new");
	ft1.setWeight(12);
#ifdef WINDOWS
	ft1.setPixelSize(97);
#endif
#ifdef LINUX
	ft1.setPixelSize(83);
#endif
	m_percentLabel->setFont(ft1);
	m_percentLabel->setPalette(labelPalette);
	m_percentLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

	m_copyTimer = new QTimer(m_copyWidget);
	connect(m_copyTimer, &QTimer::timeout, this, &MainWidget::doCopyTimerUnpress);

	m_copyPainter = new QPainter(m_copyWidget);
	m_copyPainter->setRenderHint(QPainter::Antialiasing, true);
	m_copyPainter->setPen(QPen(QColor(98, 98, 98), 4, Qt::SolidLine));

	m_percentLabel->setGeometry(220, 230, 100, 140);

	m_mainStackedLayout->addWidget(m_copyWidget);
}

void MainWidget::createFinishCopyWidget()//拷贝完成界面
{
	m_finishCopyWidget = new ZC_install();
	m_finishCopyWidget->setSkinName(QString(":/finishCopy/background"));

	connect(m_finishCopyWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_finishCopyWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);

	m_finishExprButton = new PushButton(m_finishCopyWidget);
	if (1 == isInstallClient())
	{
		m_finishExprButton->setPicName(QString(":/updateCopy/updateFinishButton")); //更新按钮背景
	}
	else if (0 == isInstallClient())
	{
		m_finishExprButton->setPicName(QString(":/finishCopy/experienceButton")); // 没有更新，安装安扭背景图
	}

	connect(m_finishExprButton, &PushButton::clicked, this, &MainWidget::doFinishExprButtonSlot);

	m_finishExprButton->setGeometry(162, 488, 208, 40);
	m_mainStackedLayout->addWidget(m_finishCopyWidget);
}

void MainWidget::createUpdateWidget()//更新界面
{
	m_updateWidget = new ZC_install();
	m_updateWidget->setSkinName(QString(":/updateCopy/background"));
	connect(m_updateWidget->m_miniButton, &PushButton::clicked, this, &MainWidget::showMinimized);
	connect(m_updateWidget->m_closeButton, &PushButton::clicked, this, &MainWidget::doCloseMainWidgetSlot);

	m_updateButton = new PushButton(m_updateWidget);
	m_updateButton->setPicName(QString(":/updateCopy/updateButton"));
	connect(m_updateButton, &PushButton::clicked, this, &MainWidget::doShowNextWidgetSlot);

	m_updateButton->setGeometry(162, 488, 208, 40);
	m_mainStackedLayout->addWidget(m_updateWidget);
}

void MainWidget::doShowNextWidgetSlot()
{
	if (!m_setPathUserDefWid->isHidden())
	{
		m_setPathUserDefWid->hide();
		setFixedSize(540, 600);
		m_setPathParentWid->setFixedSize(540, 600);
	}
	m_mainStackedLayout->setCurrentWidget(m_copyWidget);
	m_copyWidget->setdrawArc(true);
	m_copyTimer->start(90); //*** 解压缩时间比例  150
	if (1 == isInstallClient())
	{
		m_copyWidget->m_closeButton->setEnabled(false);//安装路劲已经在isInstallClient() 中赋值
	}
	else if (0 == isInstallClient())
	{
		m_copyWidget->m_closeButton->setEnabled(false);
		m_copyWidget->m_closeButton->setPicName(QString(":/system/close_hover"));
		m_installPath = m_userDefPath->displayText();
	}
	
	mainWidgetDoDelete(1); //开始解压之后，释放一些堆内存
	mainWidgetDoDelete(2);
}

void MainWidget::doShowProtocolContentSlot()
{
	if (!m_setPathUserDefWid->isHidden())
		this->setFixedSize(540, 600);
	m_mainStackedLayout->setCurrentWidget(m_licenceWidget);
}

void MainWidget::doSwitchAgreeButtonSlot()
{
	static bool checked = true;
	if (checked)
	{
		m_agreeButton->setPicName(":/beforeCopy/uncheck");
		m_agreeButton->update();
		checked = false;
 		m_setPathInstallButton->setEnabled(false);
	}
	else
	{
		m_agreeButton->setPicName(":/beforeCopy/check");
		m_agreeButton->update();
		checked = true;
		m_setPathInstallButton->setEnabled(true);
	}
}

void MainWidget::doCopyTimerUnpress() //解压
{
	//meybe different
#ifdef WINDOWS

	static HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_ZHICLOUD_ZIP), _T("ZIP"));
	static HANDLE hglob = LoadResource(NULL, hrsrc);
	static void *zipbuf = LockResource(hglob);
	static unsigned int ziplen = SizeofResource(NULL, hrsrc);
	static HZIP hz = OpenZip(zipbuf, ziplen, 0);

	//static HZIP hz = OpenZip(_T("Resources//empty.zip"), 0);//Here are discarded
	static ZIPENTRY ze; GetZipItem(hz, -1, &ze);
	static int numitems = ze.index;
	static int zi = 0;
	// -1 gives overall information about the zipfile

	if (zi < numitems)
	{
		ZIPENTRY ze; GetZipItem(hz, zi, &ze); // fetch individual details
		QString str = m_installPath + QString("\\");
		TCHAR* namePath = NULL;
		TCHAR szTBuff[1024] = { 0 };

		QStringToTCHER(&namePath, str);//qstring转换成TCHAR
		_tcscpy(szTBuff, namePath);
		if (namePath)
		{
			delete[] namePath;
			namePath = NULL;
		}
		_tcscat_s(szTBuff, ze.name);
		UnzipItem(hz, zi, szTBuff);         // e.g. the item's name.
		zi++;
		//更新画图
		int percent = (double)zi / (double)numitems * 100.0;
		if (100 == percent) percent = 99;
		m_percentLabel->setText(QString("%1").arg(percent));
		m_copyWidget->setPercent(percent);
	//	m_copyWidget->update();

	}
	else
	{
		CloseZip(hz);
		m_copyTimer->stop();
		CreateLinkAndRegedit(2);//实参暂时保留没有用
		m_isFinish = true;  //安装解压完成
		m_mainStackedLayout->setCurrentWidget(m_finishCopyWidget);
	}
#endif
#ifdef LINUX
	static QString in_file_path = QString("zhicloud.zip");
	static QString out_file_path = m_installPath;
	static QuaZip archive(in_file_path);
	static int sta = 1;
	if (1 == sta)
	{
		while (!archive.open(QuaZip::mdUnzip));
	}
	static int numitems = archive.getEntriesCount();;
	static int zi = 0;

	static QString path = out_file_path;
	static QDir dir(out_file_path);
	if (1 == sta)
	{
		if (!path.endsWith("/") && !out_file_path.endsWith("\\"))
			path += "/";
		if (!dir.exists())
			dir.mkpath(out_file_path);
		sta++;
	}
	static bool f = archive.goToFirstFile();
	//for (f; f; f = archive.goToNextFile())
	if (f)
	{
		QString filePath = archive.getCurrentFileName();
		QuaZipFile zFile(archive.getZipName(), filePath);
		zFile.open(QIODevice::ReadOnly);
		QByteArray ba = zFile.readAll();
		zFile.close();

		if (filePath.endsWith("/"))
		{
			dir.mkpath(filePath);
		}
		else
		{
			QFile dstFile(path + filePath);
// 			if (!dstFile.open(QIODevice::WriteOnly))
// 				return false;
			dstFile.open(QIODevice::WriteOnly);
			dstFile.write(ba);
			dstFile.close();
		}
		zi++;
		//更新画图
		int percent = (double)zi / (double)numitems * 100.0;
		m_percentLabel->setText(QString("%1").arg(percent));
		m_copyWidget->setPercent(percent);

		f = archive.goToNextFile();
	}
	else
	{
		m_copyTimer->stop();
		//CreateLinkAndRegedit(2);//实参暂时保留没有用
		QProcess *chmodProcess = new QProcess();
		QStringList str;
		QString fileInstall = m_installPath + QString("/zhicloud/cloud_client");
		str << "+x" << fileInstall;
		chmodProcess->start("chmod", str);
		createConfigureini();
		createDesktopLink();
		delete chmodProcess;
		chmodProcess = NULL;
		chmodProcess = new QProcess();
		QStringList strUnstall;
		QString fileUnstall = m_installPath + QString("/zhicloud/uninst");
		strUnstall << "+x" << fileUnstall;
		chmodProcess->start("chmod", strUnstall);
		m_isFinish = true;  //安装解压完成
		m_mainStackedLayout->setCurrentWidget(m_finishCopyWidget);
		delete chmodProcess;
		chmodProcess = NULL;
	}

#endif
}

void MainWidget::doExtendSetPathWidgetSlot()
{
	if (m_setPathUserDefWid->isHidden())
	{
		m_setPathUserDefWid->show();
		m_setPathParentWid->setFixedSize(540, 653);
		setFixedSize(540, 653);
	}
	else
	{
		m_setPathUserDefWid->hide();
		setFixedSize(540, 600);
		m_setPathParentWid->setFixedSize(540, 600);

	}
	
	qDebug() << "............";
}

void MainWidget::doCloseMainWidgetSlot()
{
	if (!m_isFinish)
	{
#ifdef WINDOWS
		QMessageBox msgBox(QMessageBox::Question, QString(QStringLiteral("致云")), QStringLiteral("你确定要退出“致云客户端”安装程序吗？"), QMessageBox::NoButton, this);
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
			mainWidgetDoDelete(3);
			mainWidgetDoDelete(4);
			exit(0);
		}
		else if (QMessageBox::RejectRole == ret)//取消按钮
		{

		}
	}
	else
	{
		close();
		mainWidgetDoDelete(1);
		mainWidgetDoDelete(2);
		mainWidgetDoDelete(3);
		mainWidgetDoDelete(4);
		exit(0);
	}
		
}

bool MainWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (m_licenceButton == obj)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
			if (!m_setPathUserDefWid->isHidden())
				this->setFixedSize(540, 653);
			m_mainStackedLayout->setCurrentWidget(m_setPathParentWid);
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

void MainWidget::doSelectUserDefPathSlot()
{
	QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();//meybe different
	QString defaultPath("");
	for (int count = 5; (defaultPath.isEmpty()) && (count > 0); count++)
	{
#ifdef WINDOWS
		defaultPath = environment.value(QString("ProgramFiles(x86)"), QString(""));
#endif
#ifdef LINUX
		defaultPath = QString("/usr/share");
#endif
	}

	//获取已经存在的目录路径
#ifdef WINDOWS
	QString dir = QFileDialog::getExistingDirectory(this, QStringLiteral("致云"),\
		"/home",\
		QFileDialog::ShowDirsOnly\
		| QFileDialog::DontResolveSymlinks);
#endif
#ifdef LINUX
	QFile licence;  //^^^
	licence.setFileName(":/txt/title");
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString title = tc->toUnicode(ba);
	licence.close();
	QString dir = QFileDialog::getExistingDirectory(this, title, \
		"/home", \
		QFileDialog::ShowDirsOnly\
		| QFileDialog::DontResolveSymlinks);
#endif
	if (!dir.isEmpty())
	{
#ifdef WINDOWS
		dir.replace(QString("/"), QString("\\"));
		m_userDefPath->setText(dir);
#endif

#ifndef WINDOWS
		m_userDefPath->setText(dir);
#endif
	}
	else
	{
		m_userDefPath->setText(defaultPath);
	}

}

void MainWidget::doFinishExprButtonSlot()
{
// 	QString exePath = m_installPath + QString("\\zhicloud\\cloud_client.exe");
// 	QProcess *pre = new QProcess();
// 	QStringList list; 
// 	list << m_installPath + QString("\\zhicloud\\bin") << m_installPath + QString("\\zhicloud\\share") << m_installPath + QString("\\zhicloud\\configure.ini") << m_installPath + QString("\\zhicloud\\install.exe") << m_installPath + QString("\\zhicloud\\Resources\\login_res") << m_installPath + QString("\\zhicloud\\icon") << m_installPath + QString("\\zhicloud\\qt_zh_CN.qm");
// 	bool ret = pre->startDetached(exePath, list);
// 	while (!ret)
// 	{
// 		ret = pre->startDetached(exePath, list);
// 	}
// 	//pre->start(exePath, QStringList());
// 	delete pre;
// 	pre = NULL;

#ifdef WINDOWS
	QString exePath = m_installPath + QString("\\zhicloud");
	TCHAR *tmp = NULL;
	QStringToTCHER(&tmp, exePath);
	ShellExecute(NULL, NULL, _T("cloud_client.exe"), NULL, tmp, SW_SHOW);//立即体验启动exe
	close();
	delete tmp;
	tmp = NULL;
	exit(0);
// 	SHELLEXECUTEINFO sei;
// 	TCHAR szModule[MAX_PATH], szComspec[MAX_PATH], szParams[MAX_PATH];
// 	//获取文件路径名。 
// 	if (GetEnvironmentVariable(_T("COMSPEC"), szComspec, MAX_PATH) != 0)
// 	{
// 		//设置命令行参数。 
// 		//lstrcpy(szParams, _T("/c del "));
// 		lstrcat(szParams, tmp);
// 		lstrcat(szParams, _T(" > nul"));
// 
// 		//初始化SHELLEXECUTEINFO结构成员 
// 		sei.cbSize = sizeof(sei); //设置类型大小。
// 		sei.hwnd = 0; //命令窗口进程句柄，ShellExecuteEx函数执行时设置。 
// 		sei.lpVerb = _T("Open"); //执行动作为“打开执行”。 
// 		sei.lpFile = szComspec; //执行程序文件全路径名称。 
// 		sei.lpParameters = szParams; //执行参数。 
// 		sei.lpDirectory = 0;
// 		sei.nShow = SW_HIDE; //显示方式，此处使用隐藏方式阻止出现命令窗口界面。 
// 		sei.fMask = SEE_MASK_NOCLOSEPROCESS; //设置为SellExecuteEx函数结束后进程退出。 
// 		//创建执行命令窗口进程。 
// 		if (ShellExecuteEx(&sei))
// 		{
// 			//设置命令行进程的执行级别为空闲执行，这使本程序有足够的时间从内存中退出。 
// 			SetPriorityClass(sei.hProcess, IDLE_PRIORITY_CLASS);
// 			//设置本程序进程的执行级别为实时执行，这本程序马上获取CPU执行权，快速退出
// 			SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
// 			SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
// 			//通知Windows资源浏览器，本程序文件已经被删除。
// 			SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, szModule, NULL);
// 			//执行退出程序。 
// 		}
// 	}
#endif
#ifdef LINUX
	QString execPath = m_installPath + QStringLiteral("/zhicloud/thinloginclient");
	QProcess::startDetached(execPath, QStringList());
	close();
	exit(0);
#endif
}

void MainWidget::mainWidgetDoDelete(int whichWidget)
{
	//..................................协议界面
	if (1 == whichWidget)
	{
		if (NULL != m_licenceButton)
		{
			delete m_licenceButton;
			m_licenceButton = NULL;
		}
		if (NULL != m_licenceLabel)
		{
			delete m_licenceLabel;
			m_licenceLabel = NULL;
		}
		if (NULL != m_licenceScroll)
		{
			delete m_licenceScroll;
			m_licenceScroll = NULL;
		}
		if (NULL != m_licenceWidget)
		{
			delete m_licenceWidget;
			m_licenceWidget = NULL;
		}
	}
	else if (2 == whichWidget) //.............设置界面
	{
		if (NULL != m_setPathInstallButton) //立即安装按钮
		{
			delete m_setPathInstallButton;
			m_setPathInstallButton = NULL;
		}
		if (NULL != m_protocolLabel) //“用户许可协议”按钮
		{
			delete m_protocolLabel;
			m_protocolLabel = NULL;
		}
		if (NULL != m_agreeLebel)  //“同意”
		{
			delete m_agreeLebel;
			m_agreeLebel = NULL;
		}
		if (NULL != m_agreeButton)  //“同意”选择框
		{
			delete m_agreeButton;
			m_agreeButton = NULL;
		}
		if (NULL != m_userDefLabel)   //“自定义”
		{
			delete m_userDefLabel;
			m_userDefLabel = NULL;
		}
		if (NULL != m_userDefButton)   //“自定义”按钮
		{
			delete m_userDefButton;
			m_userDefButton = NULL;
		}
	} 
	else if (3 == whichWidget)  //.....................用户自定义选择路径界面
	{
		if (NULL != m_userSelectPath)   //用户选择路径按钮
		{
			delete m_userSelectPath;
			m_userSelectPath = NULL;
		}
		if (NULL != m_userDefPath)   //显示安装路径
		{
			delete m_userDefPath;
			m_userDefPath = NULL;
		}
	}
	else if (4 == whichWidget)
	{
		//.....................解压界面
		if (NULL != m_percentLabel)
		{
			delete m_percentLabel;
			m_percentLabel = NULL;
		}
		if (NULL != m_copyTimer)
		{
			delete m_copyTimer;
			m_copyTimer = NULL;
		}
		if (NULL != m_copyPainter)
		{
			delete m_copyPainter;
			m_copyPainter = NULL;
		}

		if (NULL != m_psl)
		{
			delete m_psl;
			m_psl = NULL;
		}
		if (NULL != m_pslUnintall)
		{
			delete m_pslUnintall;
			m_pslUnintall = NULL;
		}

		if (NULL != m_finishExprButton)
		{
			delete m_finishExprButton;
			m_finishExprButton = NULL;
		}
		if (NULL != m_updateButton)
		{
			delete m_updateButton;
			m_updateButton = NULL;
		}
		if (NULL != m_mainStackedLayout)
		{
			delete m_mainStackedLayout;
			m_mainStackedLayout = NULL;
		}
		if (NULL != m_setPathLayout)
		{
			delete m_setPathLayout;
			m_setPathLayout = NULL;
		}
		if (NULL != m_setPathWidget)
		{
			delete m_setPathWidget;
			m_setPathWidget = NULL;
		}
		if (NULL != m_setPathUserDefWid)
		{
			delete m_setPathUserDefWid;
			m_setPathUserDefWid = NULL;
		}
		if (NULL != m_copyWidget)
		{
			delete m_copyWidget;
			m_copyWidget = NULL;
		}
		if (NULL != m_finishCopyWidget)
		{
			delete m_finishCopyWidget;
			m_finishCopyWidget = NULL;
		}
		if (NULL != m_updateWidget)
		{
			delete m_updateWidget;
			m_updateWidget = NULL;
		}
		if (NULL != m_setPathParentWid)
		{
			delete m_setPathParentWid;
			m_setPathParentWid = NULL;
		}
	}
}

#ifdef WINDOWS

void MainWidget::setLinkPath()
{
	HRESULT	r;
	CoInitialize(NULL);
	r = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&m_psl);
	if (SUCCEEDED(r))
	{
		TCHAR *ExePath = NULL;
		QString addPath("\\zhicloud\\cloud_client.exe");
		QString tmpExePath = m_installPath + addPath;
		QStringToTCHER(&ExePath, tmpExePath);
		m_psl->SetPath(ExePath);
		if (NULL != ExePath)
		{
			delete[] ExePath;
			ExePath = NULL;
		}
		tmpExePath = m_installPath + addPath.left(addPath.lastIndexOf(QString("\\")));
		QStringToTCHER(&ExePath, tmpExePath);
		m_psl->SetWorkingDirectory(ExePath);
		if (NULL != ExePath)
		{
			delete[] ExePath;
			ExePath = NULL;
		}
		m_psl->SetDescription(_T("致云客户端"));
	}

	HRESULT r1;
	CoInitialize(NULL);
	r1 = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&m_pslUnintall);
	if (SUCCEEDED(r1))
	{
		TCHAR *ExePath = NULL;
		QString addPath("\\zhicloud\\uninst.exe");
		QString tmpExePath = m_installPath + addPath;
		QStringToTCHER(&ExePath, tmpExePath);
		m_pslUnintall->SetPath(ExePath);
		if (NULL != ExePath)
		{
			delete[] ExePath;
			ExePath = NULL;
		}
		tmpExePath = m_installPath + addPath.left(addPath.lastIndexOf(QString("\\")));
		QStringToTCHER(&ExePath, tmpExePath);
		m_pslUnintall->SetWorkingDirectory(ExePath);
		if (NULL != ExePath)
		{
			delete[] ExePath;
			ExePath = NULL;
		}
		m_pslUnintall->SetDescription(_T("Uninstall 致云客户端"));
	}
}

BOOL MainWidget::CreateLnk(IShellLink * psl, UINT uiType, QString strLnkName, BOOL bCreateDir)
{
	LPITEMIDLIST PIDL;
	SHGetSpecialFolderLocation(0, uiType, &PIDL);
	TCHAR dir[MAX_PATH];
	ZeroMemory(dir, MAX_PATH);
	SHGetPathFromIDList(PIDL, dir);

	TCHAR LinkFileName[MAX_PATH] = { 0 };
	TCHAR IconFileName[MAX_PATH] = { 0 };

	_tcscpy(LinkFileName, dir);

	if (bCreateDir)
	{
		WIN32_FIND_DATA findUpdate;
		TCHAR linkTmp[MAX_PATH] = { 0 };
		_tcscpy(linkTmp, dir);
		_tcscat(linkTmp, L"\\致云客户端\\*.*");
		HANDLE hFind = FindFirstFile(linkTmp, &findUpdate);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			ZeroMemory(linkTmp, MAX_PATH);
			_tcscpy(linkTmp, dir);
			_tcscat(linkTmp, L"\\致云客户端");
			CreateDirectory(linkTmp, NULL);
		}
		else
		{
			FindClose(hFind);
		}
		TCHAR *tmp = NULL;
		QStringToTCHER(&tmp, strLnkName);
		//LinkFileName = LinkFileName + _T("\\致云客户端\\") + strLnkName;
		_tcscat(LinkFileName, L"\\致云客户端\\");
		_tcscpy(LinkFileName + _tcslen(LinkFileName), tmp);
		if (NULL != tmp)
		{
			delete[] tmp;
			tmp = NULL;
		}
	}
	else
	{
		TCHAR *tmp = NULL;
		QStringToTCHER(&tmp, strLnkName);
		//LinkFileName = LinkFileName + _T("\\") + strLnkName;
		_tcscat(LinkFileName, L"\\");
		_tcscpy(LinkFileName + _tcslen(LinkFileName), tmp);
		if (NULL != tmp)
		{
			delete[] tmp;
			tmp = NULL;
		}
	}
	TCHAR *tmp = NULL;
	QStringToTCHER(&tmp, m_installPath);
//	IconFileName = m_strInstallDir + _T("\\zhicloud\\icon\\logo.ico");
	_tcscpy(IconFileName, tmp);
	if (NULL != tmp)
	{
		delete[] tmp;
		tmp = NULL;
	}
	_tcscat(IconFileName, L"\\zhicloud\\icon\\logo.ico");
	HRESULT r;
	IPersistFile *ppf;
	psl->SetIconLocation(IconFileName, 0);
	r = psl->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf);
	if (SUCCEEDED(r))
	{
		r = ppf->Save(LinkFileName, TRUE);
		ppf->Release();
		return TRUE;
	}
	return TRUE;
}

void MainWidget::CreateRegedit(TCHAR *pversions, TCHAR *pPath)
{
	HKEY hKey;
	LONG iRet;
	DWORD dwDisp;

	iRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\zhicloud_client"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE | KEY_READ | KEY_WOW64_64KEY, NULL, &hKey, &dwDisp);

	RegSetValueEx(hKey, _T("versions"), 0, REG_SZ, (LPBYTE)pversions, 20);

 	RegSetValueEx(hKey, _T("path"), 0, REG_SZ, (LPBYTE)pPath, MAX_PATH);

	RegCloseKey(hKey);
}

BOOL MainWidget::CreateLinkAndRegedit(int updateOrInstall)
{
	setLinkPath();
	QString str = QStringLiteral("致云客户端.lnk");
	CreateLnk(m_psl, CSIDL_PROGRAMS, str, TRUE);

	str = QStringLiteral("Uninstall 致云客户端.lnk");
	CreateLnk(m_pslUnintall, CSIDL_PROGRAMS, str, TRUE);

	str = QStringLiteral("致云客户端.lnk");
	CreateLnk(m_psl, CSIDL_DESKTOPDIRECTORY, str, FALSE);
	//ShellExecute(NULL, _T("open"), m_strInstallDir + _T("\\zhicloud\\cloud_client.exe"), NULL, NULL, SW_SHOW);
	m_psl->Release();
	m_pslUnintall->Release();
	//添加注册表
	QString   tmpInstallPath = m_installPath + QString("\\zhicloud");
	str = tmpInstallPath + QString("\\configure.ini");
	QSettings settings(str, QSettings::IniFormat);
	QString   tmpVersions = (settings.value("UpdateInfo/version")).toString();
// 	QSettings *reg = new QSettings("HKEY_LOCAL_MACHINE//SOFTWARE",
// 		QSettings::NativeFormat);
// 	reg->setValue("zhicloud/", QString("zhicloud"));
// 	delete reg;
// 	reg = new QSettings("HKEY_LOCAL_MACHINE//SOFTWARE//zhicloud",
// 		QSettings::NativeFormat);
// 	reg->setValue("path", tmpInstallPath);
// 	reg->setValue("versions", tmpVersions);
// 	delete reg;
	TCHAR     *versions = NULL;
	TCHAR     *installPath = NULL;
	QStringToTCHER(&versions, tmpVersions);
	QStringToTCHER(&installPath, tmpInstallPath);
	CreateRegedit(versions, installPath);
	if (NULL != versions)
	{
		delete[] versions;
		versions = NULL;
	}
	if (NULL != installPath)
	{
		delete[] installPath;
		installPath = NULL;
	}
	return 0;
}

void MainWidget::QStringToTCHER(TCHAR** ppchar, const QString &str)
{
	QByteArray byt = str.toUtf8();
	char *p = byt.data();
	char tmp[MAX_PATH] = { 0 };
	memmove(tmp, p, byt.length());
	TCHAR *pp = *ppchar;
	
	DWORD dwNum = MultiByteToWideChar(CP_UTF8, 0, tmp, -1, NULL, 0);
	TCHAR *pwText;
	pwText = new TCHAR[dwNum];
	if (!pwText)
	{
		delete[]pwText;
	}
	MultiByteToWideChar(CP_UTF8, 0, tmp, -1, pwText, dwNum);

	*ppchar = pwText;
	
}

#endif  //WINDOWS

int MainWidget::isInstallClient()
{
#ifdef WINDOWS
	unsigned char chbuf[260];
	HKEY hKey;
	DWORD type = REG_SZ;
	DWORD len = 3;
	memset(chbuf, 0, sizeof(chbuf));
	LONG re = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\zhicloud_client"), 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	if (ERROR_SUCCESS == re)
	{
		RegQueryValueEx(hKey, _T("path"), NULL, &type, NULL, &len);
		BYTE *buff = new BYTE[len];
		memset(buff, 0, sizeof(buff) / sizeof(BYTE));
		if (!RegQueryValueEx(hKey, _T("path"), 0, &type, buff, &len))
		{
			m_installPath.clear();
			for (DWORD i = 0; i < len; i++)
			{
				if ('\0' == (char)buff[i])
					continue;
				else
					m_installPath.append((char)(buff[i]));
			}
			m_installPath.append('\0');
			m_installPath = m_installPath.left(m_installPath.indexOf(QString("\\zhicloud")));
		}
		delete[] buff;
		RegCloseKey(hKey);
		return 1;
	}
	else
	{
		return 0;
	}
#endif
#ifdef LINUX
	QString file = QString("/etc/zhicloud/configure.ini");
	if (QFileInfo::exists(file))
	{
		QSettings *configIniRead = new QSettings(QString("/etc/zhicloud/configure.ini"), QSettings::IniFormat);
		m_installPath = configIniRead->value("EtcConfig/path").toString();
		m_installPath = m_installPath.left(m_installPath.lastIndexOf("/zhicloud"));
		delete configIniRead;
		configIniRead = NULL;
		if (m_installPath.isEmpty())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}

#endif
}

/*linux function define*/
#ifdef LINUX
bool MainWidget::createConfigureini()
{
	QSettings *configIniWrite = new QSettings("/etc/zhicloud/configure.ini", QSettings::IniFormat);
	QSettings *configIniRead = new QSettings(m_installPath+QString("/zhicloud/configure.ini"), QSettings::IniFormat);
	QString  version = configIniRead->value("UpdateInfo/version").toString();
	configIniWrite->setValue("EtcConfig/path", m_installPath+QString("/zhicloud"));
	configIniWrite->setValue("EtcConfig/versions", version);

#ifdef DEBUG
	qDebug() << "linux createConfigIni: " << m_installPath << ", version" << version; 
#endif
	delete configIniWrite;
	configIniWrite = NULL;
	delete configIniRead;
	configIniRead = NULL;
	return true;
}

bool MainWidget::createDesktopLink()
{
	QString exec = QString("Exec=") + m_installPath + QString("/zhicloud/cloud_client");  //!!!!
	QString icon = QString("Icon=") + m_installPath + QString("/zhicloud/zcloudLogo.png");
	QFile licence;  //^^^
	licence.setFileName(":/txt/title");
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString title = tc->toUnicode(ba);
	licence.close();
	QString fileName = title + QString(".desktop"); //桌面方式文件
	QString srcFile = m_installPath + QString("/zhicloud/") + fileName;
	QFile file(srcFile);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug() << "-1";
	}
	QTextStream stream(&file);
	QStringList list;
	int i = 0;
	while (!stream.atEnd())
	{
		if (3 == i)
		{
			stream.readLine();
			list.push_back(exec);
		}
		else if (4 == i)
		{
			stream.readLine();
			list.push_back(icon);
		}
		else
		{
			QString str(stream.readLine());
			list.push_back(str);
		}
		i++;
	}
	file.close();
	{
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		{
			qDebug() << "-1";
		}
		QTextStream stream(&file);
		QStringList::iterator it = list.begin();
		while (it != list.end())
		{
			stream << *it << "\n";
			it++;
		}
		file.close();
	}
	QString path = QString("/usr/share/applications/") + fileName; // QStringLiteral("/usr/share/applications/致云.desktop");
	QFile::copy(srcFile, path);
	return true;
}
#endif