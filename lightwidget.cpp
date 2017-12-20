#include "lightwidget.hpp"
#include <QValidator>

LightWidget::LightWidget(int num, Light **lights, GlobalLight *glight, QWidget *parent) : QWidget(parent)
{
	int i;

	this->numLights = num;
	this->lights = lights;
	this->glight = glight;

	vlayout_lightPanel = new QVBoxLayout(this);
	titleBar = new TitleBar(this);
	titleBar->setBackgroundColor(QColor(153, 204, 255));
	vlayout_lightPanel->addWidget(titleBar);
	vlayout_lightPanel->setContentsMargins(10, 0, 10, 15);
	/*
	// background color
	QPalette pal = palette();

	// set black background
	pal.setColor(QPalette::Background, QColor(150, 150, 30, 40));
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	*/

// light name
	// label
	lb_lightName = new QLabel(tr("Light Name: "), this);

	vlayout_lightPanel->addWidget(lb_lightName);

	// combo box
	cbb_lightName = new QComboBox(this);
	for (i = 0; i < num; ++i)
	{
		cbb_lightName->addItem(QString(lights[i]->name));
	}

	vlayout_lightPanel->addWidget(cbb_lightName);

// global light ambient
	// label
	lb_ambient = new QLabel(tr("Ambient: "), this);

	vlayout_lightPanel->addWidget(lb_ambient);

	// line edit
	le_ambient = new QLineEdit(this);
	re = new QRegExp("[(]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[)]?");
	reValidator = new QRegExpValidator(*re, this);
	le_ambient->setValidator(reValidator);
	le_ambient->setToolTip("( r, g, b, a )");

	if (numLights > 0) {
		le_ambient->setText(QString("(%1, %2, %3, %4)").arg(glight->ambient[0]).arg(glight->ambient[1]).arg(glight->ambient[2]).arg(glight->ambient[3]));
	}

	vlayout_lightPanel->addWidget(le_ambient);

// light position
	// label
	lb_lightPosition = new QLabel(tr("Position: "), this);

	vlayout_lightPanel->addWidget(lb_lightPosition);

	// line edit
	le_lightPosition = new QLineEdit(this);
	re = new QRegExp("[(]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[)]?");
	reValidator = new QRegExpValidator(*re, this);
	le_lightPosition->setValidator(reValidator);
	le_lightPosition->setToolTip(tr("( x, y, z, direct = 0 / point = 1 )"));

	if (numLights > 0) {
		le_lightPosition->setText(QString("(%1, %2, %3, %4)").arg(lights[0]->position[0]).arg(lights[0]->position[1]).arg(lights[0]->position[2]).arg(lights[0]->position[3]));
	}

	vlayout_lightPanel->addWidget(le_lightPosition);

// light direction
	// label
	lb_lightDirection = new QLabel(tr("Direction: "), this);

	vlayout_lightPanel->addWidget(lb_lightDirection);

	// line edit
	le_lightDirection = new QLineEdit(this);
	re = new QRegExp("[(]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[)]?");
	reValidator = new QRegExpValidator(*re, this);
	le_lightDirection->setToolTip(tr("( x, y, z )"));

	if (numLights > 0) {
		le_lightDirection->setText(QString("(%1, %2, %3)").arg(lights[0]->direction[0]).arg(lights[0]->direction[1]).arg(lights[0]->direction[2]));
	}

	vlayout_lightPanel->addWidget(le_lightDirection);

// light diffuse
	// label
	lb_lightDiffuse = new QLabel(tr("Diffuse"), this);

	vlayout_lightPanel->addWidget(lb_lightDiffuse);

	// line edit
	le_lightDiffuse = new QLineEdit(this);
	re = new QRegExp("[(]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[)]?");
	reValidator = new QRegExpValidator(*re, this);
	le_lightDiffuse->setValidator(reValidator);
	le_lightDiffuse->setToolTip(tr("( x, y, z, direct = 0 / point = 1 )"));

	if (numLights > 0) {
		le_lightDiffuse->setText(QString("(%1, %2, %3, %4)").arg(lights[0]->diffuse[0]).arg(lights[0]->diffuse[1]).arg(lights[0]->diffuse[2]).arg(lights[0]->diffuse[3]));
	}

	vlayout_lightPanel->addWidget(le_lightDiffuse);

// light specular
	// label
	lb_lightSpecular = new QLabel(tr("Specular"), this);

	vlayout_lightPanel->addWidget(lb_lightSpecular);

	// line edit
	le_lightSpecular = new QLineEdit(this);
	re = new QRegExp("[(]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[,]?[ ]*[-+]?[0-9]*\.?[0-9]+[ ]*[)]?");
	reValidator = new QRegExpValidator(*re, this);
	le_lightSpecular->setValidator(reValidator);
	le_lightSpecular->setToolTip(tr("( r, g, b, a )"));

	if (numLights > 0) {
		le_lightSpecular->setText(QString("(%1, %2, %3, %4)").arg(lights[0]->specular[0]).arg(lights[0]->specular[1]).arg(lights[0]->specular[2]).arg(lights[0]->specular[3]));
	}

	vlayout_lightPanel->addWidget(le_lightSpecular);

// light cutoff
	// label
	lb_lightCutoff = new QLabel(tr("Cutoff"), this);

	vlayout_lightPanel->addWidget(lb_lightCutoff);

	// line edit
	le_lightCutoff = new QLineEdit(this);
	re = new QRegExp("[0-9]*\.?[0-9]+");
	reValidator = new QRegExpValidator(*re, this);
	le_lightCutoff->setValidator(reValidator);
	le_lightCutoff->setToolTip(tr("( r, g, b, a )"));

	if (numLights > 0) {
		le_lightCutoff->setText(QString("%1").arg(lights[0]->cutoff));
	}

	vlayout_lightPanel->addWidget(le_lightCutoff);

	
	// connect
	connect(le_ambient, SIGNAL(returnPressed(void)), this, SLOT(changeAmbient(void)));
	connect(le_lightPosition, SIGNAL(returnPressed(void)), this, SLOT(changePosition(void)));
	connect(le_lightDirection, SIGNAL(returnPressed(void)), this, SLOT(changeDirection(void)));
	connect(le_lightDiffuse, SIGNAL(returnPressed(void)), this, SLOT(changeDiffuse(void)));
	connect(le_lightSpecular, SIGNAL(returnPressed(void)), this, SLOT(changeSpecular(void)));
	connect(le_lightCutoff, SIGNAL(returnPressed(void)), this, SLOT(changeCutoff(void)));
	connect(titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	connect(titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragged(int, int)));
}

LightWidget::~LightWidget() 
{
	
}

void LightWidget::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	painter.fillRect(0, 0, width() - 5, height() - 7, QColor(210, 233, 255, 80));
	painter.drawRect(0, 0, width() - 5, height() - 7);
	painter.fillRect(titleBar->rect().adjusted(1, 1, 15, 0), titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void LightWidget::changeAmbient()
{
	QString text;
	int curIdx;
	float ambient[4];

	text = this->le_ambient->text();
	curIdx = this->cbb_lightName->currentIndex();
	getNumberFromQString(text, glight->ambient);

	emit dataUpdate();
}

void LightWidget::changePosition(void)
{
	QString text;
	int curIdx;
	float position[4];

	text = this->le_lightPosition->text();
	curIdx = this->cbb_lightName->currentIndex();
	getNumberFromQString(text, lights[curIdx]->position);

	emit dataUpdate();
}

void LightWidget::changeDirection(void)
{
	QString text;
	int curIdx;
	float direction[3];

	text = this->le_lightDirection->text();
	curIdx = this->cbb_lightName->currentIndex();
	getNumberFromQString(text, lights[curIdx]->direction);

	emit dataUpdate();
}

void LightWidget::changeDiffuse(void)
{
	QString text;
	int curIdx;
	float diffuse[4];

	text = this->le_lightDiffuse->text();
	curIdx = this->cbb_lightName->currentIndex();
	getNumberFromQString(text, lights[curIdx]->diffuse);

	emit dataUpdate();
}

void LightWidget::changeSpecular(void)
{
	QString text;
	int curIdx;
	float specular[4];

	text = this->le_lightSpecular->text();
	curIdx = this->cbb_lightName->currentIndex();
	getNumberFromQString(text, lights[curIdx]->specular);

	emit dataUpdate();
}

void LightWidget::changeCutoff(void)
{
	QString text;
	int curIdx;
	float cutoff;

	text = this->le_lightCutoff->text();
	curIdx = this->cbb_lightName->currentIndex();
	getNumberFromQString(text, &lights[curIdx]->cutoff);

	emit dataUpdate();
}

void LightWidget::getNumberFromQString(const QString &string, float* values)
{
	int count;
	QStringList list = string.split("(", QString::SkipEmptyParts);
	list = list.at(0).split(")", QString::SkipEmptyParts);
	list = list.at(0).split(", ", QString::SkipEmptyParts);

	count = 0;
	for each (QString str in list)
	{
		values[count++] = str.toFloat();
	}
}

void LightWidget::closeWidget(void)
{
	this->close();
}

void LightWidget::dragged(int x, int y)
{
	int posX, posY;

	posX = this->x() + x;
	posY = this->y() + y;

	QRect boundary = this->parentWidget()->geometry();;

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
