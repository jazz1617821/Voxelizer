#include "modelviewer.hpp"
#include <QFileDialog>
#include <QString>

float g_originWidth;
float g_originHeight;

ModelViewer::ModelViewer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	g_originWidth = this->width();
	g_originHeight = this->height();

	connect(this, SIGNAL(openSTL(const char*)), ui.ogl_modelViewer, SLOT(openSTL(const char*)));
	connect(ui.ogl_modelViewer, SIGNAL(sendModelScene(MeshModelScene*)), ui.wid_modelScene, SLOT(updateModelScene(MeshModelScene*)));
	connect(ui.ogl_modelViewer, SIGNAL(selectModel(MeshModelItem*)), ui.wid_modelScene, SLOT(modelSelected(MeshModelItem*)));
	connect(ui.wid_modelScene, SIGNAL(actionOnModelObject(MeshModelObject*, int)), ui.ogl_modelViewer, SLOT(actionOnModelObject(MeshModelObject*, int)));
	connect(ui.pb_view, SIGNAL(released(void)), ui.ogl_modelViewer, SLOT(changeToViewMode(void)));
	connect(ui.pb_translate, SIGNAL(released(void)), ui.ogl_modelViewer, SLOT(changeToTranslateMode(void)));
	connect(ui.pb_rotate, SIGNAL(released(void)), ui.ogl_modelViewer, SLOT(changeToRotateMode(void)));
	connect(ui.pb_scale, SIGNAL(released(void)), ui.ogl_modelViewer, SLOT(changeToScaleMode(void)));
	connect(ui.pb_voxelize, SIGNAL(released(void)), ui.ogl_modelViewer, SLOT(showVoxelizeWidget(void)));
	connect(ui.ogl_modelViewer, SIGNAL(sendModelObject(MeshModelObject*)), this, SLOT(setModelObjectAttributePanel(MeshModelObject*)));
}

ModelViewer::~ModelViewer()
{

}

void ModelViewer::resizeEvent(QResizeEvent* e)
{
	// default function
	QMainWindow::resizeEvent(e);

}

void ModelViewer::on_actionOpen_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("All Files (*.inf *.stl *.obj);;Info Files (*.inf);;STL Files (*.stl);;Wavefront OBJ (*.obj);;"));
	QByteArray ba = filename.toLatin1();
	const char *str = ba.data();
	const char *fe;	// file extension

	fe = strrchr(str, '.');
	if (!filename.isEmpty()) {
		if (!strcmp(fe, ".stl")) {
			emit(openSTL(str));
		}
	}
}

void ModelViewer::setModelObjectAttributePanel(MeshModelObject* obj)
{
	QTreeWidget *w = ui.wid_modelAttributes;
	QTreeWidgetItem *witem, *witem2;
	QString str;
	QStringList strs;
	MeshModel *model;

	// initialize
	w->clear();

	// add attributes
	// MeshModel Object Name
	strs.append(tr("Name"));
	strs.append(obj->name);
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	// add to widget
	w->addTopLevelItem(witem);

	// MeshModel BBox
	strs.append(tr("Bounding Box"));
	witem = new QTreeWidgetItem(strs);
	strs.clear();
	strs.append(tr("Max"));
	str = QString("%1 x %2 x %3").arg(obj->bbox->max[0]).arg(obj->bbox->max[1]).arg(obj->bbox->max[2]);
	strs.append(str);
	witem2 = new QTreeWidgetItem(strs);
	strs.clear();
	witem->addChild(witem2);
	strs.append(tr("Min"));
	str = QString("%1 x %2 x %3").arg(obj->bbox->min[0]).arg(obj->bbox->min[1]).arg(obj->bbox->min[2]);
	strs.append(str);
	witem2 = new QTreeWidgetItem(strs);
	strs.clear();
	witem->addChild(witem2);
	// add to widget
	w->addTopLevelItem(witem);

	if (obj->item != NULL) {
		model = obj->item->model;

		// Number of Face
		strs.append(tr("Faces"));
		strs.append(QString::number(model->numFaces));
		witem = new QTreeWidgetItem(strs);
		strs.clear();
		// add to widget
		w->addTopLevelItem(witem);
	}

}