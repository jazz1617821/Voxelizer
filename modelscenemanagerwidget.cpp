#include "modelscenemanagerwidget.hpp"
#include <QString>
#include <QMenu>
#include <QAction>
#include <assert.h>


ModelSceneManagerItem::ModelSceneManagerItem(QStringList text, MeshModelObject* obj)
	:QTreeWidgetItem(text)
{
	this->object = obj;

}

ModelSceneManagerItem::~ModelSceneManagerItem()
{

}


ModelSceneManagerWidget::ModelSceneManagerWidget(QWidget * parent) : QWidget(parent) 
{
	// data
	sc = NULL;

	//setStyleSheet("background-color: rgba(0,0,0,0)");

// vlayout
	vlayout_managerPanel = new QVBoxLayout(this);
	//titleBar = new TitleBar(this);
	//titleBar->setBackgroundColor(QColor(153, 204, 255));
	//vlayout_managerPanel->addWidget(titleBar);
	vlayout_managerPanel->setContentsMargins(0, 0, 0, 0);

	// tree widget
	treeWidget = new QTreeWidget(this);
	QStringList strList;
	strList.append("Name");
	treeWidget->setHeaderLabels(strList);
	//treeWidget->setStyleSheet("QHeaderView::section { background-color: rgba(0,0,0,0); }");

	vlayout_managerPanel->addWidget(treeWidget);

	// set context menu
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	// connect
	connect(treeWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(toggleContextMenu(const QPoint&)));
	connect(treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(itemSelected(QTreeWidgetItem*, int)));
	//connect(titleBar, SIGNAL(closeTriggered(void)), this, SLOT(closeWidget(void)));
	//connect(titleBar, SIGNAL(dragged(int, int)), this, SLOT(dragged(int, int)));
}

ModelSceneManagerWidget::~ModelSceneManagerWidget() 
{
	
}

void ModelSceneManagerWidget::paintEvent(QPaintEvent * e)
{
	//QPainter painter(this);

	//painter.drawRect(0, 0, width() - 5, height() - 7);
	//painter.fillRect(titleBar->rect().adjusted(1, 1, 15, 0), titleBar->getBackgroundColor());

	// inherited 
	QWidget::paintEvent(e);
}

void ModelSceneManagerWidget::mousePressEvent(QMouseEvent * e)
{
	switch (e->buttons())
	{
	case Qt::LeftButton:
		break;
	case Qt::MidButton:
		break;
	case Qt::RightButton:
		 
		break;
	default:
		break;
	}
}

void ModelSceneManagerWidget::buildTree(void)
{
	if (sc == NULL) return;

	ModelSceneManagerItem *parentItem, *childItem;
	QStringList name;
	MeshModelObject *parentObject, *childObject;
	Dequeue *itemDequeue;

	treeWidget->clear();
	name = QStringList(sc->root->name);
	parentItem = new ModelSceneManagerItem(name, sc->root);
	treeWidget->addTopLevelItem(parentItem);
	itemDequeue = newDequeue();
	pushBackDequeue(itemDequeue, parentItem);
	while (!isEmptyDequeue(itemDequeue))
	{
		parentItem = (ModelSceneManagerItem*)popFrontDequeue(itemDequeue);
		parentObject = parentItem->object;
		childObject = childOfModelObject(parentObject);
		while (childObject != NULL)
		{
			name = QStringList(childObject->name);
			childItem = new ModelSceneManagerItem(name, childObject);
			parentItem->addChild(childItem);
			childObject = nextSiblingOfModelObject(childObject);
			pushBackDequeue(itemDequeue, childItem);
		}
	}
	deleteDequeue(&itemDequeue);
}

//
//		Slots
//

/*
void ModelSceneManagerWidget::closeWidget(void)
{
	this->close();
}
*/

/*
void ModelSceneManagerWidget::dragged(int x, int y)
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
*/

void ModelSceneManagerWidget::openImageProjectionWidget()
{
	if (selectedItem == NULL) return;

	QModelIndex index = treeWidget->currentIndex();

	printf("row: %d\ncol: %d\n", index.row(), index.column());
}


//
//		SLOTS
//

void ModelSceneManagerWidget::updateModelScene(MeshModelScene* sc)
{
	this->sc = sc;
	buildTree();
	update();
}

void ModelSceneManagerWidget::toggleContextMenu(const QPoint& pos)
{
	QMenu menu(tr("Actions"));

	//  Panels
	QAction action1(tr("Projection mapping"), this);
	QAction action2(tr("Delete"), this);
	//action1.setCheckable(true);
	//action1.setChecked(this->lightWidget->isVisible());
	connect(&action1, SIGNAL(triggered()), this, SLOT(openImageProjectionWidget()));
	connect(&action2, SIGNAL(triggered()), this, SLOT(deleteModelObject()));
	//menu.addAction(&action1);
	menu.addAction(&action2);

	selectedItem = treeWidget->itemAt(pos);
	if (selectedItem != NULL) {
		menu.exec(treeWidget->mapToGlobal(pos));
	}
}

void ModelSceneManagerWidget::modelSelected(MeshModelItem* item)
{
	MeshModelObject* obj = item->object;

	QList<QTreeWidgetItem*> list = this->treeWidget->findItems(obj->name, Qt::MatchContains | Qt::MatchRecursive, 0);
	foreach(QTreeWidgetItem* item, list)
	{
		ModelSceneManagerItem* modelItem = (ModelSceneManagerItem*)item;
		if (modelItem->object == obj) {
			this->treeWidget->setCurrentItem(item);
		}
	}
}

void ModelSceneManagerWidget::deleteModelObject()
{
	ModelSceneManagerItem *item;
	MeshModelObject *obj;
	QList<QTreeWidgetItem *>  items = this->treeWidget->selectedItems();
	QTreeWidgetItem          *pp = nullptr;

	if (!items.isEmpty())
	{
		item = (ModelSceneManagerItem*)selectedItem;
		obj = item->object;

		foreach(QTreeWidgetItem *item, items)
		{
			pp = item->parent();
			pp->removeChild(item);
			delete item;
		}
	}
	
	emit actionOnModelObject(obj, MOBJ_DELETE);
	update();
}

void ModelSceneManagerWidget::itemSelected(QTreeWidgetItem* witem, int index)
{
	ModelSceneManagerItem *item = (ModelSceneManagerItem*)witem;
	MeshModelObject *obj = item->object;

	emit actionOnModelObject(obj, MOBJ_SELECT);
}