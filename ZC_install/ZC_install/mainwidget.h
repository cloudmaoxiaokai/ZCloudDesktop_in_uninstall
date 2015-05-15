#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "common.h" 

#include <QCoreApplication>
#include <QWidget>
#include <QDesktopWidget>
#include <QStackedLayout>
#include <QMainWindow>
#include <QPalette>
#include <QBrush>
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>
#include <QTranslator>
#include <QScrollArea>
#include <QScrollBar>
#include <QPainterPath>
#include <QScrollBar>
#include <QFileDialog>
#include <QLineEdit>
#include <QProcess>
#include "zc_install.h"
#include "myclicklabel.h"

#ifdef WINDOWS
#include <iostream>
#include "resource.h"
#include "unzip.h"
#include <tchar.h>
#include <ShObjIdl.h>
#include <ShlObj.h>
#endif
#ifdef LINUX
#include "3rdparty/quazip/include/JlCompress.h"
#include "./3rdparty/quazip/include/quazip.h"
#include <QDir>
#include "./3rdparty/quazip/include/quazipfile.h"
#include <QSettings>
#include <QFileInfo>
#endif

// #include <JlCompress.h>  //这四个库是quazip解压
// #include <quazip.h>
// #include <QDir>
// #include <quazipfile.h>


class MainWidget : public QDialog
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent=0);
	~MainWidget();

	void showWidget(QWidget *);
	
protected:
	void createSetPathWidget();
	void createCopyWidget();
	void createFinishCopyWidget();
	void createLicenceWidget();
	void createUpdateWidget();

	bool eventFilter(QObject *, QEvent *);
protected slots:
	void doShowNextWidgetSlot();
	void doShowProtocolContentSlot();
	void doSwitchAgreeButtonSlot();
	void doExtendSetPathWidgetSlot();
	void doCloseMainWidgetSlot();
	void doSelectUserDefPathSlot();
	void doFinishExprButtonSlot();

	void doCopyTimerUnpress();
protected:
	ZC_install *m_setPathWidget;   //设置界面
	QWidget    *m_setPathParentWid;//自定义和设置界面的父类
	QWidget    *m_setPathUserDefWid;//自定义路径界面
	ZC_install *m_copyWidget;//解压界面
	ZC_install *m_finishCopyWidget;//解压完成界面
	ZC_install *m_updateWidget; //更新界面
	bool       m_isFinish;   //是否安装或更新完成

	QStackedLayout *m_mainStackedLayout;//......................设置界面     **
	PushButton     *m_setPathInstallButton;  //立即安装按钮
	MyClickLabel   *m_protocolLabel;  //“用户许可协议”按钮
	QLabel         *m_agreeLebel;  //“同意”
	CheckButton    *m_agreeButton; //“同意”选择框
	QLabel         *m_userDefLabel;     //“自定义”
	CheckButton    *m_userDefButton;  //“自定义”按钮
	QVBoxLayout    *m_setPathLayout;  //装上下两个wid      **
	PushButton     *m_userSelectPath; //用户选择路径按钮
	QLineEdit      *m_userDefPath;   //显示安装路径
	QString        m_installPath; //最后使用的安装路径   **

	ZC_install     *m_licenceWidget;  //...............许可协议界面
	QLabel         *m_licenceLabel;
	PushButton     *m_licenceButton;
	QScrollArea    *m_licenceScroll;

	QLabel         *m_percentLabel;//.....................解压界面
	QTimer         *m_copyTimer;
	QPainter       *m_copyPainter;
#ifdef WINDOWS
	IShellLink	   *m_psl;
	IShellLink	   *m_pslUnintall;
#endif

	PushButton     *m_finishExprButton;//..................解压完成

	PushButton     *m_updateButton;//..................更行界面的立即更新按钮

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	QPoint     m_movePoint;
	bool       m_mousePress;
private:
#ifdef WINDOWS
	void setLinkPath();
	BOOL CreateLnk(IShellLink * psl, UINT uiType, QString strLnkName, BOOL bCreateDir);
	void CreateRegedit(TCHAR *pversions, TCHAR *pPath);
	BOOL CreateLinkAndRegedit(int updateOrInstall);  //实参暂时保留没有用
	void QStringToTCHER(TCHAR** ppchar, const QString &str);
#endif
	int isInstallClient();
#ifdef LINUX
	bool createConfigureini(); //记录安装位置和版本
	bool createDesktopLink(); //桌面快捷方式
#endif
	void mainWidgetDoDelete(int whichWidget);

};

#endif // MAINWIDGET_H
