#ifndef ZC_unstall_H
#define ZC_unstall_H

#include "common.h"

#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <qmath.h>
#include <QPoint>
#include <QtWidgets/QDialog>
#include <QPainterPath>
#include <QFont>
#include <QSysInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QSettings>
#include "push_button.h"

#ifdef LINUX
#include <QTextCodec>
#endif

#define PAI 3.14

class ZC_unstall : public QWidget
{
	Q_OBJECT

public:
	explicit ZC_unstall(QWidget *parent = 0);
	~ZC_unstall();

	void setSkinName(const QString skinName) { m_skinName = skinName; }
	PushButton *m_closeButton;
	PushButton *m_miniButton;
	QLabel     *m_versionLabel;
	void setdrawArc(bool b);
	void setPercent(double percent);
protected:
	void paintEvent(QPaintEvent *event);
	QPointF calcEndPoint(int alen);
	bool m_openDrawArc; //�Ƿ�򿪻�Բ
	int m_startAngle;
protected slots:
	void doUpdate();

private:
	QString    m_skinName;
	QTimer     *m_updateTimer;
	int        m_percent;
};

#endif // ZC_unstall_H
