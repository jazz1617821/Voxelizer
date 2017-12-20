#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QtWidgets/QMainWindow>
#include <QResizeEvent>
#include "ui_modelviewer.h"

class ModelViewer : public QMainWindow
{
	Q_OBJECT

public:
	ModelViewer(QWidget *parent = 0);
	~ModelViewer();

protected:
	void resizeEvent(QResizeEvent* e);

signals:
	void openSTL(const char* fileName);
	

public slots:
	void on_actionOpen_triggered(void);
	void setModelObjectAttributePanel(MeshModelObject* obj);

private:
	Ui::ModelViewerClass ui;

};

#endif // MODELVIEWER_H
