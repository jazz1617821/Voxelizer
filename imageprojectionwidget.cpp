#include "imageprojectionwidget.hpp"
#include <QFileDialog>
#include <QCursor>
#include <math.h>


#define POW2(x) ((x) * (x))

TextureWidget::TextureWidget(QWidget * parent) : QWidget(parent)
{
	originImage = NULL;
	image = NULL;
	isResize = false;
}

TextureWidget::~TextureWidget()
{

}

void TextureWidget::paintEvent(QPaintEvent * e)
{
	QPainter painter(this);

	if (image != NULL) {
		painter.drawImage(0, 0, *image);
	}

	painter.drawRect(0, 0, width() - 1, height() - 1);

	// inherited 
	QWidget::paintEvent(e);
}

void TextureWidget::resizeEvent(QResizeEvent* e)
{
	if (image != NULL && isResize) {
		*image = originImage->scaled(e->size());
	}

	emit imageUpdated(image);

	QWidget::resizeEvent(e);
}

void TextureWidget::moveEvent(QMoveEvent * e)
{
	emit imageUpdated(image);
	
	QWidget::moveEvent(e);
}

void TextureWidget::showEvent(QShowEvent * e)
{
	if (image != NULL) {
		*image = originImage->scaled(this->size());
	}

	emit imageUpdated(image);

	QWidget::showEvent(e);
}

int TextureWidget::openFile(const char * fileName)
{
	if (originImage != NULL)
		delete originImage;

	originImage = new QImage(fileName);
	if (originImage == NULL) return 0;

	resetImage();
	
	update();
	return 1;
}

void TextureWidget::resetImage(void)
{
	bool whMax;
	float ratio;

	if (originImage == NULL) {
		this->move(0, 0);
		this->resize(100, 30);
		return;
	}
	if (image != NULL) {
		delete image;
	}
	image = new QImage(*originImage);

	// max
	if (originImage->width() > originImage->height()) {
		whMax = 0;
		//ratio = image->width() / (float)image->height();
	}
	else {
		whMax = 1;
		//ratio = image->height() / (float)image->width();
	}

	// scale image
	if (whMax = 0 && originImage->width() >= this->parentWidget()->width()) {
		ratio = (this->parentWidget()->width() - 1) / (float)originImage->width();
		*image = originImage->scaled(this->parentWidget()->width() - 1, (int)(ratio * originImage->height()));
	}
	else if (whMax = 1 && originImage->height() >= this->parentWidget()->height()) {
		ratio = (this->parentWidget()->height() - 1) / (float)originImage->height();
		*image = originImage->scaled((int)(ratio * originImage->width()), this->parentWidget()->height());
	}

	this->move(0, 0);
	this->resize(image->width(), image->height());
}

void TextureWidget::checkBoundary()
{
	QRect boundary = this->parentWidget()->rect();

	if (this->pos().x() < boundary.topLeft().x()) {
		this->move(boundary.topLeft().x(), this->pos().y());
	}
	if (this->pos().y() < boundary.topLeft().y()) {
		this->move(this->pos().x(), boundary.topLeft().y());
	}
	if (this->pos().x() + this->width() > boundary.bottomRight().x()) {
		this->move(boundary.bottomRight().x() - this->width(), this->pos().y());
	}
	if (this->pos().y() + this->height() > boundary.bottomRight().y()) {
		this->move(this->pos().x(), boundary.bottomRight().y() - this->height());
	}
}

void TextureWidget::activeResize()
{
	isResize = true;
}

void TextureWidget::inactiveResize()
{
	isResize = false;
}





//


TextureMappingWidget::TextureMappingWidget(QWidget * parent) : QWidget(parent)
{
	textureWidget = new TextureWidget(this);
	textureWidgetSub = new TextureWidget(this);
	textureWidgetSub->hide();
	this->setMinimumSize(480, 270);

	cycleState = 0;
	resizeCircleRadius = 10;
	resizeCircle = -1;
	texture = new QImage(480, 270, QImage::Format_RGB32);

	// connect
	connect(textureWidget, SIGNAL(imageUpdated(QImage*)), this, SLOT(imageUpdated(QImage*)));
	connect(textureWidgetSub, SIGNAL(imageUpdated(QImage*)), this, SLOT(imageUpdated(QImage*)));
}

TextureMappingWidget::~TextureMappingWidget()
{

}

void TextureMappingWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	QRect boundary = textureWidget->geometry();

	painter.drawRect(0, 0, width() - 1, height() - 1);

	painter.setPen(QPen(QColor(1, 0, 0)));
	painter.drawEllipse(boundary.topLeft(), resizeCircleRadius, resizeCircleRadius);
	painter.drawEllipse(boundary.topRight(), resizeCircleRadius, resizeCircleRadius);
	painter.drawEllipse(boundary.bottomLeft(), resizeCircleRadius, resizeCircleRadius);
	painter.drawEllipse(boundary.bottomRight(), resizeCircleRadius, resizeCircleRadius);

	if (boundary.topLeft().x() < 0) {
		printf("POSX: %d\n", boundary.topLeft().x());
	}

	// inherited 
	QWidget::paintEvent(e);
}


QPoint TextureMappingWidget::checkBoundary(int x, int y)
{
	QRect boundary = this->rect();
	QPoint pos(x, y);

	if (x < boundary.topLeft().x()) {
		pos.setX(boundary.topLeft().x());
	}
	if (y < boundary.topLeft().y()) {
		pos.setY(boundary.topLeft().y());
	}
	if (x > boundary.bottomRight().x()) {
		pos.setX(boundary.bottomRight().x());
	}
	if (y > boundary.bottomRight().y()) {
		pos.setY(boundary.bottomRight().y());
	}

	QCursor::setPos(mapToGlobal(pos));

	return pos;
}

QSize TextureMappingWidget::checkResize(int width, int height)
{
	QSize size(width, height);

	if (width < 2) {
		size.setWidth(1);
	}
	if (height < 2) {
		size.setHeight(1);
	}
	if (width > this->width()) {
		size.setWidth(this->width());
	}
	if( height > this->height()) {
		size.setHeight(this->height());
	}

	return size;
}

//  0     1
//
//  2     3
//
int TextureMappingWidget::checkResizeCircle(int x, int y)
{
	QRect boundary = textureWidget->geometry();
	int powTwoRadius = POW2(resizeCircleRadius);

	if (POW2(x - boundary.topLeft().x()) + POW2(y - boundary.topLeft().y()) < powTwoRadius) {
		return 0;
	}
	else if (POW2(x - boundary.topRight().x()) + POW2(y - boundary.topRight().y()) < powTwoRadius) {
		return 1;
	}
	else if (POW2(x - boundary.bottomLeft().x()) + POW2(y - boundary.bottomLeft().y()) < powTwoRadius) {
		return 2;
	}
	else if (POW2(x - boundary.bottomRight().x()) + POW2(y - boundary.bottomRight().y()) < powTwoRadius) {
		return 3;
	}
	
	return -1;
}

void TextureMappingWidget::mousePressEvent(QMouseEvent* e)
{
	if (e->buttons() == Qt::LeftButton) {
		oldMouseX = e->x();
		oldMouseY = e->y();
		resizeCircle = checkResizeCircle(e->x(), e->y());
		if (resizeCircle >= 0) {
			textureWidget->activeResize();
			textureWidgetSub->activeResize();
		}
	}
	update();
}

void TextureMappingWidget::mouseMoveEvent(QMouseEvent* e)
{
	int ret, deltaX, deltaY, width, height, posX, posY;
	QPoint widgetPos = textureWidget->pos();					// widget positoin
	QPoint modifiedPos;											// lock mouse in the widget
	QSize modifiedSize;											// lock size in widget and can not smaller than 1 pixel

	if (e->buttons() == Qt::LeftButton) {
		// resize texture
		if (resizeCircle >= 0) {
			modifiedPos = checkBoundary(e->x(), e->y());
		}
		switch (resizeCircle)
		{
		case 0:
			width = textureWidget->width() - modifiedPos.x() + oldMouseX;
			height = textureWidget->height() - modifiedPos.y() + oldMouseY;
			posX = widgetPos.x() + modifiedPos.x() - oldMouseX;
			posY = widgetPos.y() + modifiedPos.y() - oldMouseY;
			break;
		case 1:
			width = textureWidget->width() + modifiedPos.x() - oldMouseX;
			height = textureWidget->height() - modifiedPos.y() + oldMouseY;
			posX = widgetPos.x();
			posY = widgetPos.y() + modifiedPos.y() - oldMouseY;
			break;
		case 2:
			width = textureWidget->width() - modifiedPos.x() + oldMouseX;
			height = textureWidget->height() + modifiedPos.y() - oldMouseY;
			posX = widgetPos.x() + modifiedPos.x() - oldMouseX;
			posY = widgetPos.y();
			break;
		case 3:
			width = textureWidget->width() + modifiedPos.x() - oldMouseX;
			height = textureWidget->height() + modifiedPos.y() - oldMouseY;
			posX = widgetPos.x();
			posY = widgetPos.y();
			break;
		default:
			break;
		}
		if (resizeCircle >= 0) {
			modifiedSize = checkResize(width, height);
			textureWidget->resize(modifiedSize);
			textureWidget->move(posX, posY);
			textureWidgetSub->resize(modifiedSize);
			if (cycleState == 1) {
				textureWidgetSub->move(posX + this->width(), posY);
			}
			else if (cycleState == 2) {
				textureWidgetSub->move(posX - this->width(), posY);
			}
			oldMouseX = modifiedPos.x();
			oldMouseY = modifiedPos.y();
			update();
			return;
		}
		// move texture
		if (textureWidget->underMouse()) {
			modifiedPos.setX(textureWidget->pos().x() + e->x() - oldMouseX);
			modifiedPos.setY(textureWidget->pos().y() + e->y() - oldMouseY);
			if (modifiedPos.y() < 0) {
				modifiedPos.setY(0);
			}
			else if (modifiedPos.y() + textureWidget->height() > this->height()) {
				modifiedPos.setY(this->height() - textureWidget->height());
			}
			if (modifiedPos.x() < 0) {
				cycleState = 1;
				textureWidgetSub->show();
				textureWidgetSub->move(modifiedPos.x() + this->width(), modifiedPos.y());
			}
			else if (modifiedPos.x() + textureWidget->width() > this->width()) {
				cycleState = 2;
				textureWidgetSub->show();
				textureWidgetSub->move(modifiedPos.x() - this->width(), modifiedPos.y());
			}
			else {
				cycleState = 0;
				textureWidgetSub->hide();
			}
			textureWidget->move(modifiedPos);
		}
	}

	oldMouseX = e->x();
	oldMouseY = e->y();
	update();
}

void TextureMappingWidget::mouseReleaseEvent(QMouseEvent* e)
{
	
	if (e->button() == Qt::LeftButton) {
		textureWidget->inactiveResize();
		textureWidgetSub->inactiveResize();
	}
}

int TextureMappingWidget::openFile(const char * fileName)
{
	int ret;
	
	ret = textureWidget->openFile(fileName);
	if (ret > 0) {
		textureWidgetSub->openFile(fileName);
	}
	return ret;
}

void TextureMappingWidget::resetImage(void)
{
	textureWidget->resetImage();
	textureWidgetSub->resetImage();
	this->repaint();
}

//
//		SLOTS
//

void TextureMappingWidget::imageUpdated(QImage* image)
{
	QPainter painter;
	QWidget* widget = (QWidget*)QObject::sender();
	static int count = 0;
	
	if (count == 0) {
		texture->fill(QColor(255, 255, 255, 0));
	}
	
	++count;
	if (image == NULL) return;
	painter.begin(texture);
	painter.drawImage(widget->pos().x(), widget->pos().y(), *image);
	painter.end();

	
	if (cycleState && count < 2) {
		return;
	}
	if (count >= 2) {
		count = 0;
	}
	emit textureUpdated(texture);
}




//

ImageProjectionWidget::ImageProjectionWidget(QWidget * parent) : QWidget(parent) 
{
	vlayout_panel = new QVBoxLayout(this);
	titleBar = new TitleBar(this);
	titleBar->setBackgroundColor(QColor(153, 204, 255));
	vlayout_panel->addWidget(titleBar);
	vlayout_panel->setContentsMargins(10, 0, 10, 15);

// Buttons
	// hlayout
	hlayuout = new QHBoxLayout();

	// open
	pb_open = new QPushButton(tr("Open"), this);
	hlayuout->addWidget(pb_open);

	// reset
	pb_resetImage = new QPushButton(tr("Reset"), this);
	hlayuout->addWidget(pb_resetImage);

	vlayout_panel->addLayout(hlayuout);

// texture mapping
	textureWidget = new TextureMappingWidget(this);

	vlayout_panel->addWidget(textureWidget);

	// connect
	connect(pb_open, SIGNAL(released(void)), this, SLOT(openFile(void)));
	connect(pb_resetImage, SIGNAL(released(void)), this, SLOT(resetImage(void)));
	connect(titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragged(int, int)));
	connect(titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(textureWidget, SIGNAL(textureUpdated(QImage*)), this, SLOT(textureUpdated(QImage*)));
}

ImageProjectionWidget::~ImageProjectionWidget() 
{
	
}

void ImageProjectionWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(titleBar->rect().adjusted(1, 1, 15, 0), titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void ImageProjectionWidget::mousePressEvent(QMouseEvent* e)
{
	
}

//
//	SLOTS
//

void ImageProjectionWidget::openFile(void)
{
	int ret;
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString());
	QByteArray ba = fileName.toLatin1();
	const char *str = ba.data();

	ret = textureWidget->openFile(str);
}

void ImageProjectionWidget::resetImage(void)
{
	textureWidget->resetImage();
}

void ImageProjectionWidget::closeWidget(void)
{
	this->close();
}

void ImageProjectionWidget::dragged(int x, int y)
{
	int posX, posY;

	posX = this->x() + x;
	posY = this->y() + y;

	QRect boundary = this->parentWidget()->geometry();

	if (posX < boundary.topLeft().x()) {
		posX = boundary.topLeft().x();
	}
	else if (posY < boundary.topLeft().y()) {
		posY = boundary.topLeft().y();
	}
	else if (posX > boundary.bottomRight().x() - titleBar->width()) {
		posX = boundary.bottomRight().x() - titleBar->width();
	}
	else if (posY > boundary.bottomRight().y() - titleBar->height()) {
		posY = boundary.bottomRight().y() - titleBar->height();
	}

	move(posX, posY);
}

void ImageProjectionWidget::textureUpdated(QImage * texture)
{
	emit sendTexture(texture);
}
