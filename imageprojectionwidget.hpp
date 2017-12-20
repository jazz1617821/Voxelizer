#pragma once
#include "titlebar.hpp"
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QImage>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QShowEvent>
#include <QResizeEvent>

class TextureWidget : public QWidget
{
	Q_OBJECT

public:
	TextureWidget(QWidget * parent = Q_NULLPTR);
	~TextureWidget();
	void paintEvent(QPaintEvent* e);
	void resizeEvent(QResizeEvent* e);
	void moveEvent(QMoveEvent* e);
	void showEvent(QShowEvent* e);

	int openFile(const char* fileName);
	void resetImage(void);
	void checkBoundary();
	void activeResize();
	void inactiveResize();

signals:
	void imageUpdated(QImage* image);

private:
	QImage* originImage;
	QImage* image;
	int oldMouseX;
	int oldMouseY;
	int oldPosX;
	int oldPosY;
	bool isResize;
};

class TextureMappingWidget : public QWidget
{
	Q_OBJECT

public:
	TextureMappingWidget(QWidget * parent = Q_NULLPTR);
	~TextureMappingWidget();
	void paintEvent(QPaintEvent* e);

	int openFile(const char* fileName);
	void resetImage(void);

protected:
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);

signals:
	void textureUpdated(QImage* texture);

public slots:
	void imageUpdated(QImage* image);

private:
	QPoint checkBoundary(int x, int y);
	QSize checkResize(int width, int height);
	int checkResizeCircle(int x, int y);

private:
	TextureWidget* textureWidget;
	TextureWidget* textureWidgetSub;

	QImage* texture;
	int cycleState;
	int resizeCircle;
	int resizeCircleRadius;
	int oldMouseX;
	int oldMouseY;
};

class ImageProjectionWidget : public QWidget 
{
	Q_OBJECT

public:
	ImageProjectionWidget(QWidget * parent = Q_NULLPTR);
	~ImageProjectionWidget();
	void paintEvent(QPaintEvent* e);

protected:
	void mousePressEvent(QMouseEvent* e);


signals:
	void sendTexture(QImage* texture);

public slots:
	void openFile(void);
	void closeWidget(void);
	void dragged(int x, int y);
	void textureUpdated(QImage* texture);
	void resetImage(void);

private:
	TitleBar* titleBar;
	QVBoxLayout* vlayout_panel;
	QHBoxLayout* hlayuout;
	QPushButton* pb_open;
	QPushButton* pb_resetImage;
	QPushButton* pb_confirm;
	TextureMappingWidget* textureWidget;
};
