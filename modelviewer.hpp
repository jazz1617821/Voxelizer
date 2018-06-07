#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <QtWidgets/QMainWindow>
#include <QResizeEvent>
#include "ui_modelviewer.h"
#include "voxelizewidget.hpp"

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
	void getVoxelModel(VoxelModel*);
	void on_actionOpen_triggered(void);
	void on_actionRaw_triggered(void);
	void on_actionVoxelModel_triggered(void);
	void setModelObjectAttributePanel(MeshModelObject* obj);


private:
	Ui::ModelViewerClass ui;
	VoxelModel* vmodel;

};

#endif // MODELVIEWER_H
