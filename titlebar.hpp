#ifndef _TITLE_BAR_H
#define _TITLE_BAR_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QStyle>
#include <QLayout>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>

class TitleBar : public QWidget
{
	Q_OBJECT
public:
	TitleBar(QWidget *parent);

	// events
	void paintEvent(QPaintEvent* e);

	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);

	void setBackgroundColor(QColor color);
	QColor getBackgroundColor(void);

signals:
	void closeTriggered(void);
	void dragged(int x, int y);
	void released(int x, int y);

private slots:
	void triggerCloseSignal(void);

private:
	QPushButton *close;
	QColor backgroundColor;

	// mouse
	int mouseX;
	int mouseY;
};

#endif
