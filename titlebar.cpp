#include "titlebar.hpp"
#include <QDrag>
#include <QMimeData>

TitleBar::TitleBar(QWidget *parent = 0) : QWidget(parent)
{
	QStyle *style = qApp->style();
	QIcon closeIcon = style->standardIcon(QStyle::SP_TitleBarCloseButton);
	//QIcon maxIcon = style->standardIcon(QStyle::SP_TitleBarMaxButton);
	//QIcon minIcon = style->standardIcon(QStyle::SP_TitleBarMinButton);

	//QPushButton *min = new QPushButton(this);
	//QPushButton *max = new QPushButton(this);
	close = new QPushButton(this);
	//min->setIcon(minIcon);
	//max->setIcon(maxIcon);
	close->setIcon(closeIcon);

	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->addStretch();
	//layout->addWidget(min);
	//layout->addWidget(max);
	layout->addWidget(close);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);
	connect(close, SIGNAL(released()), this, SLOT(triggerCloseSignal()));
}

void TitleBar::triggerCloseSignal(void)
{
	emit closeTriggered();
}

void TitleBar::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

	painter.fillRect(1, 1, this->width(), this->height(), backgroundColor);

	QWidget::paintEvent(e);
}

void TitleBar::mousePressEvent(QMouseEvent* e)
{
	mouseX = e->globalX();
	mouseY = e->globalY();
};

void TitleBar::mouseMoveEvent(QMouseEvent* e)
{
	if (mouseX < 0) return;

	const int distanceX = e->globalX() - mouseX;
	const int distanceY = e->globalY() - mouseY;

	mouseX = e->globalX();
	mouseY = e->globalY();

	emit dragged(distanceX, distanceY);
};

void TitleBar::mouseReleaseEvent(QMouseEvent* e)
{
	mouseX = -1;

	emit released(e->x(), e->y());
}

void TitleBar::setBackgroundColor(QColor color)
{
	backgroundColor = color;
	update();
}

QColor TitleBar::getBackgroundColor(void)
{
	return backgroundColor;
}