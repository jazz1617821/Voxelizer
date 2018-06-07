/********************************************************************************
** Form generated from reading UI file 'modelviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEWER_H
#define UI_MODELVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "modelscenemanagerwidget.hpp"
#include "modelvieweroglwidget.hpp"

QT_BEGIN_NAMESPACE

class Ui_ModelViewerClass
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionVoxelModel;
    QAction *actionRaw;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tab_mainWidget;
    QWidget *tab_model;
    QVBoxLayout *verticalLayout_4;
    QSplitter *sph_modelViewAndTool;
    QSplitter *sph_modelManagerAndView;
    QWidget *wid_modelManager;
    QVBoxLayout *verticalLayout_2;
    QSplitter *spv_modelSceneAndAttributes;
    ModelSceneManagerWidget *wid_modelScene;
    QTreeWidget *wid_modelAttributes;
    ModelViewerOGLWidget *ogl_modelViewer;
    QScrollArea *sa_modelTools;
    QWidget *sa_modelToolsContents;
    QGridLayout *gridLayout;
    QSpacerItem *spc_modelToolsContents;
    QPushButton *pb_translate;
    QPushButton *pb_view;
    QPushButton *pb_rotate;
    QPushButton *pb_scale;
    QPushButton *pb_voxelize;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuExport;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelViewerClass)
    {
        if (ModelViewerClass->objectName().isEmpty())
            ModelViewerClass->setObjectName(QStringLiteral("ModelViewerClass"));
        ModelViewerClass->resize(600, 400);
        actionOpen = new QAction(ModelViewerClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionQuit = new QAction(ModelViewerClass);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionVoxelModel = new QAction(ModelViewerClass);
        actionVoxelModel->setObjectName(QStringLiteral("actionVoxelModel"));
        actionRaw = new QAction(ModelViewerClass);
        actionRaw->setObjectName(QStringLiteral("actionRaw"));
        centralWidget = new QWidget(ModelViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tab_mainWidget = new QTabWidget(centralWidget);
        tab_mainWidget->setObjectName(QStringLiteral("tab_mainWidget"));
        tab_model = new QWidget();
        tab_model->setObjectName(QStringLiteral("tab_model"));
        verticalLayout_4 = new QVBoxLayout(tab_model);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        sph_modelViewAndTool = new QSplitter(tab_model);
        sph_modelViewAndTool->setObjectName(QStringLiteral("sph_modelViewAndTool"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sph_modelViewAndTool->sizePolicy().hasHeightForWidth());
        sph_modelViewAndTool->setSizePolicy(sizePolicy);
        sph_modelViewAndTool->setOrientation(Qt::Horizontal);
        sph_modelManagerAndView = new QSplitter(sph_modelViewAndTool);
        sph_modelManagerAndView->setObjectName(QStringLiteral("sph_modelManagerAndView"));
        sizePolicy.setHeightForWidth(sph_modelManagerAndView->sizePolicy().hasHeightForWidth());
        sph_modelManagerAndView->setSizePolicy(sizePolicy);
        sph_modelManagerAndView->setOrientation(Qt::Horizontal);
        wid_modelManager = new QWidget(sph_modelManagerAndView);
        wid_modelManager->setObjectName(QStringLiteral("wid_modelManager"));
        sizePolicy.setHeightForWidth(wid_modelManager->sizePolicy().hasHeightForWidth());
        wid_modelManager->setSizePolicy(sizePolicy);
        wid_modelManager->setMinimumSize(QSize(50, 0));
        wid_modelManager->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_2 = new QVBoxLayout(wid_modelManager);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        spv_modelSceneAndAttributes = new QSplitter(wid_modelManager);
        spv_modelSceneAndAttributes->setObjectName(QStringLiteral("spv_modelSceneAndAttributes"));
        spv_modelSceneAndAttributes->setOrientation(Qt::Vertical);
        wid_modelScene = new ModelSceneManagerWidget(spv_modelSceneAndAttributes);
        wid_modelScene->setObjectName(QStringLiteral("wid_modelScene"));
        wid_modelScene->setMinimumSize(QSize(0, 200));
        spv_modelSceneAndAttributes->addWidget(wid_modelScene);
        wid_modelAttributes = new QTreeWidget(spv_modelSceneAndAttributes);
        wid_modelAttributes->setObjectName(QStringLiteral("wid_modelAttributes"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(wid_modelAttributes->sizePolicy().hasHeightForWidth());
        wid_modelAttributes->setSizePolicy(sizePolicy1);
        spv_modelSceneAndAttributes->addWidget(wid_modelAttributes);

        verticalLayout_2->addWidget(spv_modelSceneAndAttributes);

        sph_modelManagerAndView->addWidget(wid_modelManager);
        ogl_modelViewer = new ModelViewerOGLWidget(sph_modelManagerAndView);
        ogl_modelViewer->setObjectName(QStringLiteral("ogl_modelViewer"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(ogl_modelViewer->sizePolicy().hasHeightForWidth());
        ogl_modelViewer->setSizePolicy(sizePolicy2);
        ogl_modelViewer->setMinimumSize(QSize(400, 0));
        sph_modelManagerAndView->addWidget(ogl_modelViewer);
        sph_modelViewAndTool->addWidget(sph_modelManagerAndView);
        sa_modelTools = new QScrollArea(sph_modelViewAndTool);
        sa_modelTools->setObjectName(QStringLiteral("sa_modelTools"));
        sa_modelTools->setMaximumSize(QSize(82, 16777215));
        sa_modelTools->setFrameShape(QFrame::NoFrame);
        sa_modelTools->setFrameShadow(QFrame::Sunken);
        sa_modelTools->setLineWidth(1);
        sa_modelTools->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        sa_modelTools->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        sa_modelTools->setWidgetResizable(true);
        sa_modelToolsContents = new QWidget();
        sa_modelToolsContents->setObjectName(QStringLiteral("sa_modelToolsContents"));
        sa_modelToolsContents->setGeometry(QRect(0, 0, 65, 322));
        gridLayout = new QGridLayout(sa_modelToolsContents);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        spc_modelToolsContents = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spc_modelToolsContents, 5, 0, 1, 1);

        pb_translate = new QPushButton(sa_modelToolsContents);
        pb_translate->setObjectName(QStringLiteral("pb_translate"));
        pb_translate->setMinimumSize(QSize(64, 64));
        pb_translate->setMaximumSize(QSize(64, 64));

        gridLayout->addWidget(pb_translate, 1, 0, 1, 1);

        pb_view = new QPushButton(sa_modelToolsContents);
        pb_view->setObjectName(QStringLiteral("pb_view"));
        pb_view->setMinimumSize(QSize(64, 64));
        pb_view->setMaximumSize(QSize(64, 64));

        gridLayout->addWidget(pb_view, 0, 0, 1, 1);

        pb_rotate = new QPushButton(sa_modelToolsContents);
        pb_rotate->setObjectName(QStringLiteral("pb_rotate"));
        pb_rotate->setMinimumSize(QSize(64, 64));
        pb_rotate->setMaximumSize(QSize(64, 64));

        gridLayout->addWidget(pb_rotate, 2, 0, 1, 1);

        pb_scale = new QPushButton(sa_modelToolsContents);
        pb_scale->setObjectName(QStringLiteral("pb_scale"));
        pb_scale->setMinimumSize(QSize(64, 64));
        pb_scale->setMaximumSize(QSize(64, 64));

        gridLayout->addWidget(pb_scale, 3, 0, 1, 1);

        pb_voxelize = new QPushButton(sa_modelToolsContents);
        pb_voxelize->setObjectName(QStringLiteral("pb_voxelize"));
        pb_voxelize->setMinimumSize(QSize(64, 64));
        pb_voxelize->setMaximumSize(QSize(64, 64));

        gridLayout->addWidget(pb_voxelize, 4, 0, 1, 1);

        sa_modelTools->setWidget(sa_modelToolsContents);
        sph_modelViewAndTool->addWidget(sa_modelTools);

        verticalLayout_4->addWidget(sph_modelViewAndTool);

        tab_mainWidget->addTab(tab_model, QString());

        verticalLayout->addWidget(tab_mainWidget);

        ModelViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ModelViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuExport = new QMenu(menuFile);
        menuExport->setObjectName(QStringLiteral("menuExport"));
        ModelViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ModelViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ModelViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ModelViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ModelViewerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(menuExport->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuExport->addAction(actionVoxelModel);
        menuExport->addAction(actionRaw);

        retranslateUi(ModelViewerClass);

        tab_mainWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ModelViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *ModelViewerClass)
    {
        ModelViewerClass->setWindowTitle(QApplication::translate("ModelViewerClass", "Voxelizer", nullptr));
        actionOpen->setText(QApplication::translate("ModelViewerClass", "Open", nullptr));
        actionQuit->setText(QApplication::translate("ModelViewerClass", "Quit", nullptr));
        actionVoxelModel->setText(QApplication::translate("ModelViewerClass", "Voxel Model", nullptr));
        actionRaw->setText(QApplication::translate("ModelViewerClass", "Raw", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = wid_modelAttributes->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("ModelViewerClass", "Value", nullptr));
        ___qtreewidgetitem->setText(0, QApplication::translate("ModelViewerClass", "Attribute", nullptr));
        pb_translate->setText(QApplication::translate("ModelViewerClass", "Translate", nullptr));
        pb_view->setText(QApplication::translate("ModelViewerClass", "View", nullptr));
        pb_rotate->setText(QApplication::translate("ModelViewerClass", "Rotate", nullptr));
        pb_scale->setText(QApplication::translate("ModelViewerClass", "Scale", nullptr));
        pb_voxelize->setText(QApplication::translate("ModelViewerClass", "Voxelize", nullptr));
        tab_mainWidget->setTabText(tab_mainWidget->indexOf(tab_model), QApplication::translate("ModelViewerClass", "Model", nullptr));
        menuFile->setTitle(QApplication::translate("ModelViewerClass", "File", nullptr));
        menuExport->setTitle(QApplication::translate("ModelViewerClass", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelViewerClass: public Ui_ModelViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEWER_H
