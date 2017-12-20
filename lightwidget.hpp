#ifndef _LIGHT_WIDGET_H
#define _LIGHT_WIDGET_H
extern "C" {
#include "light.h"
}
#include "titlebar.hpp"
#include <QLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialog>
#include <QLineEdit>
#include <QPaintEvent>
#include <QPainter>

class LightWidget : public QWidget {
	Q_OBJECT

public:
	LightWidget(int num, Light **lights, GlobalLight *glight, QWidget *parent = Q_NULLPTR);
	~LightWidget();
	void getNumberFromQString(const QString &string, float* values);
	void paintEvent(QPaintEvent *e);

signals:
	void dataUpdate(void);

public slots:
	void changeAmbient(void);
	void changePosition(void);
	void changeDirection(void);
	void changeDiffuse(void);
	void changeSpecular(void);
	void changeCutoff(void);

private slots:
	void closeWidget(void);
	void dragged(int x, int y);

private:
	TitleBar* titleBar;
	QVBoxLayout* vlayout_lightPanel;
	QHBoxLayout* hlayout;
	QLabel* lb_lightName;
	QLabel* lb_lightDiffuse;
	QLabel* lb_lightSpecular;
	QLabel* lb_lightPosition;
	QLabel* lb_lightDirection;
	QLabel* lb_lightCutoff;
	QLabel* lb_ambient;
	QLineEdit* le_lightDiffuse;
	QLineEdit* le_lightSpecular;
	QLineEdit* le_lightPosition;
	QLineEdit* le_lightDirection;
	QLineEdit* le_lightCutoff;
	QLineEdit* le_ambient;
	QPushButton* pb_confirm;
	QPushButton* pb_cancel;
	QPushButton* pb_newLight;
	QPushButton* pb_deleteLight;
	QComboBox* cbb_lightName;
	QRegExpValidator *reValidator;
	QRegExp* re;
private:
	int numLights;
	Light **lights;
	GlobalLight *glight;
};

#endif
