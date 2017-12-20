#pragma once
extern "C" {
#include "meshmodel.h"
}
#include "titlebar.hpp"
#include "voxelizewidget.hpp"
#include <QWidget>
#include <QLayout>
#include <QTreeWidget>
#include <QMouseEvent>
#include <QStringList>

enum MODEL_SCENE_OBJ_ACTIONS { MOBJ_SELECT, MOBJ_DELETE };

class ModelSceneManagerItem : public QTreeWidgetItem
{
public:
	ModelSceneManagerItem(QStringList text, MeshModelObject *obj);
	~ModelSceneManagerItem();

public:
	MeshModelObject *object;
};

class ModelSceneManagerWidget : public QWidget {
	Q_OBJECT

public:
	ModelSceneManagerWidget(QWidget  *parent = Q_NULLPTR);
	~ModelSceneManagerWidget();
	void paintEvent(QPaintEvent *e);

protected:
	void mousePressEvent(QMouseEvent *e);

private:
	void buildTree(void);

signals:
	void actionOnModelObject(MeshModelObject *obj, int action);
	void sendModelScene(MeshModelScene *sc);

public slots:
	// context menu
	void updateModelScene(MeshModelScene *sc);
	void toggleContextMenu(const QPoint &pos);
	void modelSelected(MeshModelItem *item);
	void itemSelected(QTreeWidgetItem* witem, int index);

private slots:
	//void closeWidget(void);
	//void dragged(int x, int y);
	void openImageProjectionWidget();
	void deleteModelObject();

private:
	//TitleBar *titleBar;
	QLayout *vlayout_managerPanel;
	QTreeWidget *treeWidget;

	QTreeWidgetItem *selectedItem;

private:
	MeshModelScene *sc;
};
