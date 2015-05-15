#include "zc_unstall.h"

QString g_zcSysFontFamily;

ZC_unstall::ZC_unstall(QWidget *parent)
	:QWidget(parent)
{
#ifdef WINDOWS
	int version = QSysInfo::windowsVersion();
	if (version == QSysInfo::WV_XP)
	{
		g_zcSysFontFamily = QStringLiteral("宋体");
	}
	else if (version >= QSysInfo::WV_VISTA)
	{
		g_zcSysFontFamily = QStringLiteral("微软雅黑");
	}
	else
	{
		g_zcSysFontFamily = QStringLiteral("微软雅黑");
	}
#endif
#ifdef LINUX
	g_zcSysFontFamily = QStringLiteral("微软雅黑");
#endif

	setFixedSize(540, 600);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	//setAttribute(Qt::WA_TranslucentBackground);

	m_skinName = "";
	m_openDrawArc = false;
	m_startAngle = 0;
	m_percent = 0;
	m_updateTimer = NULL;

	m_closeButton = new PushButton(this);
	m_miniButton  = new PushButton(this);
	m_closeButton->setPicName(QString(":/system/close"));
	m_miniButton->setPicName(QString(":/system/minimize"));

	m_versionLabel = new QLabel(this);
	QFont ft;
	ft.setFamily(g_zcSysFontFamily);
	ft.setPixelSize(12);
	QPalette labelPalette;
	labelPalette.setColor(QPalette::WindowText, QColor(72, 194, 253));
	m_versionLabel->setFont(ft);
	m_versionLabel->setPalette(labelPalette);
	QSettings settings("configure.ini", QSettings::IniFormat);
	QString   tmpVersions = (settings.value("UpdateInfo/version")).toString();
#ifdef WINDOWS
	QString v(QStringLiteral("致云科技v") + tmpVersions);
#endif
#ifdef LINUX
	QFile licence(":/txt/zcTitle");  //^^^
	licence.open(QFile::ReadOnly);
	QTextCodec *tc = QTextCodec::codecForName("GBK");//支持中文
	QByteArray ba = licence.readAll();
	QString tmp = tc->toUnicode(ba);
	QString v(tmp + tmpVersions);
#endif
	m_versionLabel->setText(v);

	m_closeButton->setGeometry(518, 11, 11, 11);
	m_miniButton->setGeometry(497, 16, 11, 1);
	m_versionLabel->setGeometry(10, 10, 100, 12);
}

ZC_unstall::~ZC_unstall()
{
	if (NULL != m_updateTimer)
	{
		delete m_updateTimer;
		m_updateTimer = NULL;
	}
}

void ZC_unstall::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if (m_skinName != QString(""))
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::white);
		painter.drawPixmap(QRect(0, 0, this->width(), this->height()), QPixmap(m_skinName));
	}

	if (m_openDrawArc)
	{

		painter.setRenderHint(QPainter::Antialiasing, true);
		QConicalGradient conicalGradient(0, 0, 0);
// 		conicalGradient.setColorAt(0.0, Qt::white);
// 		conicalGradient.setColorAt(90.0 / 360.0, Qt::blue);
// 		conicalGradient.setColorAt(180.0 / 360.0, QColor(46, 10, 248));
// 		conicalGradient.setColorAt(270.0 / 360.0, QColor(248, 25, 66));
// 		conicalGradient.setColorAt(0.99, Qt::yellow);
// 		conicalGradient.setColorAt(1.0, Qt::white);
		conicalGradient.setColorAt(0.0, Qt::white);
		conicalGradient.setColorAt(60.0 / 360.0, Qt::yellow);
		conicalGradient.setColorAt(120.0 / 360.0, Qt::green);
		conicalGradient.setColorAt(180.0 / 360.0, Qt::cyan);
		conicalGradient.setColorAt(240.0 / 360.0, Qt::blue);
		conicalGradient.setColorAt(300.0 / 360.0, Qt::magenta);
		conicalGradient.setColorAt(1.0, Qt::white);


// 		QLinearGradient linearGradient(60, 50, 200, 200);
// 		linearGradient.setColorAt(0.2, Qt::white);
// 		linearGradient.setColorAt(0.6, Qt::green);
// 		linearGradient.setColorAt(1.0, Qt::black);
// 		painter.setPen(QPen(QBrush(linearGradient), 3, Qt::SolidLine, Qt::RoundCap));
		painter.setPen(QPen(QBrush(conicalGradient), 3, Qt::SolidLine, Qt::RoundCap));
		painter.drawArc(132, 155, 274, 274, 90 * 16, -m_percent * 16);

		QPen pen(QColor(250, 251, 252), 1, Qt::SolidLine, Qt::RoundCap);
		pen.setWidthF(1.3);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		qreal r = 2;
		QPointF center = calcEndPoint(m_percent);
		painter.drawEllipse(center, r + 0.8, r + 0.8);
		pen.setWidthF(1);
		pen.setColor(QColor(57, 195, 242));
		painter.setPen(pen);
		painter.drawEllipse(center, r + 2.6, r + 2.6);
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(0, 255, 58)));
		painter.drawEllipse(center, r, r);
	}
}

void ZC_unstall::setdrawArc(bool b)
{
	m_openDrawArc = b;
	m_updateTimer = new QTimer();
	connect(m_updateTimer, &QTimer::timeout, this, &ZC_unstall::doUpdate);
	m_updateTimer->start(1);//***  界面刷新比例
}
		
void ZC_unstall::setPercent(double percent)
{
	double angle = 0.0;
	if (percent < 100)
	{
		angle = percent/100.0 * 360.0;
	}
	else
	{
		angle = 360;
	}
	
	m_startAngle = angle;
	update();
}

QPointF ZC_unstall::calcEndPoint(int alen)
{
	qreal x = 0;
	qreal y = 0;
	if (alen <= 90)
	{
		x = 137 * qSin((alen)*PAI / 180) + 269;
		y = -137 * qCos((alen)*PAI / 180) + 292;
	}
	else if (alen > 90 && alen <= 180)
	{
		x = 137 * qCos((alen - 90)*PAI / 180) + 269;
		y = 137 * qSin((alen - 90)*PAI / 180) + 292;
	}
	else if (alen > 180 && alen <= 270)
	{
		x = -137 * qSin((alen-180)*PAI / 180) + 269;
		y = 137 * qCos((alen-180)*PAI / 180) + 292;
	}
	else
	{
		x = -137 * qCos((alen-270)*PAI / 180) + 269;
		y = -137 * qSin((alen-270)*PAI / 180) + 292;
	}

	return QPointF(x, y);
}

void ZC_unstall::doUpdate()
{
// 	if (m_startAngle >= m_percent)
// 	{
// 		m_percent++;
// 		update();
// 	}
	if (m_percent < 360)
	{
		m_percent++;
		update();
	}
}