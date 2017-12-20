#pragma once
extern "C" {
#include "voxelize.h"
}
#include "ui_voxelizewidget.h"
#include <QWidget>

class VoxelizeWidget : public QWidget {
	Q_OBJECT

public:
	VoxelizeWidget(QWidget *parent = Q_NULLPTR);
	~VoxelizeWidget();
	void paintEvent(QPaintEvent *e);
	void setModelScene(MeshModelScene *scene);

private slots:
	void closeWidget(void);
	void dragWidget(int x, int y);
	void dragDone(int x, int y);
	void confirm(void);
	void cancel(void);
	void changeVoxelSize(void);
	void changeVoxelResolution(void);
	void changeSolid(int state);

private:
	void moveWidget(int x, int y);

private:
	Ui::VoxelizeWidget ui;
	MeshModelScene *scene;
	float voxSize[3];
	float voxRes[3];
	bool isSolid;
};
