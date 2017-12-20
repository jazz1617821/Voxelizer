/********************************************************************************
** Form generated from reading UI file 'voxelizewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOXELIZEWIDGET_H
#define UI_VOXELIZEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "titlebar.hpp"

QT_BEGIN_NAMESPACE

class Ui_VoxelizeWidget
{
public:
    QVBoxLayout *vlayout_widget;
    TitleBar *titleBar;
    QLabel *lb_voxelSize;
    QHBoxLayout *hlayout_voxelSize;
    QHBoxLayout *hlayout_voxelSizeX;
    QLabel *lb_voxelSizeX;
    QLineEdit *le_voxelSizeX;
    QHBoxLayout *hlayout_voxelSizeY;
    QLabel *lb_voxelSizeY;
    QLineEdit *le_voxelSizeY;
    QHBoxLayout *hlayout_voxelSizeZ;
    QLabel *lb_voxelSizeZ;
    QLineEdit *le_voxelSizeZ;
    QLabel *lb_resolution;
    QHBoxLayout *hlayout_resolution;
    QHBoxLayout *hlayout_resolutionX;
    QLabel *lb_resolutionX;
    QLineEdit *le_resolutionX;
    QHBoxLayout *hlayout_resolutionY;
    QLabel *lb_resolutionY;
    QLineEdit *le_resolutionY;
    QHBoxLayout *hlayout_resolutionZ;
    QLabel *lb_resolutionZ;
    QLineEdit *le_resolutionZ;
    QVBoxLayout *vlayout_solidModel;
    QLabel *lb_solidModel;
    QHBoxLayout *hlayout_solidModel;
    QLabel *lb_solidModelSpace;
    QCheckBox *ckb_solidModel;
    QSpacerItem *spr_widget;
    QDialogButtonBox *btmbox_widget;

    void setupUi(QWidget *VoxelizeWidget)
    {
        if (VoxelizeWidget->objectName().isEmpty())
            VoxelizeWidget->setObjectName(QStringLiteral("VoxelizeWidget"));
        VoxelizeWidget->resize(400, 200);
        vlayout_widget = new QVBoxLayout(VoxelizeWidget);
        vlayout_widget->setSpacing(6);
        vlayout_widget->setContentsMargins(11, 11, 11, 11);
        vlayout_widget->setObjectName(QStringLiteral("vlayout_widget"));
        vlayout_widget->setContentsMargins(10, 0, 10, 15);
        titleBar = new TitleBar(VoxelizeWidget);
        titleBar->setObjectName(QStringLiteral("titleBar"));

        vlayout_widget->addWidget(titleBar);

        lb_voxelSize = new QLabel(VoxelizeWidget);
        lb_voxelSize->setObjectName(QStringLiteral("lb_voxelSize"));

        vlayout_widget->addWidget(lb_voxelSize);

        hlayout_voxelSize = new QHBoxLayout();
        hlayout_voxelSize->setSpacing(6);
        hlayout_voxelSize->setObjectName(QStringLiteral("hlayout_voxelSize"));
        hlayout_voxelSizeX = new QHBoxLayout();
        hlayout_voxelSizeX->setSpacing(6);
        hlayout_voxelSizeX->setObjectName(QStringLiteral("hlayout_voxelSizeX"));
        lb_voxelSizeX = new QLabel(VoxelizeWidget);
        lb_voxelSizeX->setObjectName(QStringLiteral("lb_voxelSizeX"));

        hlayout_voxelSizeX->addWidget(lb_voxelSizeX);

        le_voxelSizeX = new QLineEdit(VoxelizeWidget);
        le_voxelSizeX->setObjectName(QStringLiteral("le_voxelSizeX"));

        hlayout_voxelSizeX->addWidget(le_voxelSizeX);


        hlayout_voxelSize->addLayout(hlayout_voxelSizeX);

        hlayout_voxelSizeY = new QHBoxLayout();
        hlayout_voxelSizeY->setSpacing(6);
        hlayout_voxelSizeY->setObjectName(QStringLiteral("hlayout_voxelSizeY"));
        lb_voxelSizeY = new QLabel(VoxelizeWidget);
        lb_voxelSizeY->setObjectName(QStringLiteral("lb_voxelSizeY"));

        hlayout_voxelSizeY->addWidget(lb_voxelSizeY);

        le_voxelSizeY = new QLineEdit(VoxelizeWidget);
        le_voxelSizeY->setObjectName(QStringLiteral("le_voxelSizeY"));

        hlayout_voxelSizeY->addWidget(le_voxelSizeY);


        hlayout_voxelSize->addLayout(hlayout_voxelSizeY);

        hlayout_voxelSizeZ = new QHBoxLayout();
        hlayout_voxelSizeZ->setSpacing(6);
        hlayout_voxelSizeZ->setObjectName(QStringLiteral("hlayout_voxelSizeZ"));
        lb_voxelSizeZ = new QLabel(VoxelizeWidget);
        lb_voxelSizeZ->setObjectName(QStringLiteral("lb_voxelSizeZ"));

        hlayout_voxelSizeZ->addWidget(lb_voxelSizeZ);

        le_voxelSizeZ = new QLineEdit(VoxelizeWidget);
        le_voxelSizeZ->setObjectName(QStringLiteral("le_voxelSizeZ"));

        hlayout_voxelSizeZ->addWidget(le_voxelSizeZ);


        hlayout_voxelSize->addLayout(hlayout_voxelSizeZ);


        vlayout_widget->addLayout(hlayout_voxelSize);

        lb_resolution = new QLabel(VoxelizeWidget);
        lb_resolution->setObjectName(QStringLiteral("lb_resolution"));

        vlayout_widget->addWidget(lb_resolution);

        hlayout_resolution = new QHBoxLayout();
        hlayout_resolution->setSpacing(6);
        hlayout_resolution->setObjectName(QStringLiteral("hlayout_resolution"));
        hlayout_resolutionX = new QHBoxLayout();
        hlayout_resolutionX->setSpacing(6);
        hlayout_resolutionX->setObjectName(QStringLiteral("hlayout_resolutionX"));
        lb_resolutionX = new QLabel(VoxelizeWidget);
        lb_resolutionX->setObjectName(QStringLiteral("lb_resolutionX"));

        hlayout_resolutionX->addWidget(lb_resolutionX);

        le_resolutionX = new QLineEdit(VoxelizeWidget);
        le_resolutionX->setObjectName(QStringLiteral("le_resolutionX"));
        le_resolutionX->setReadOnly(true);

        hlayout_resolutionX->addWidget(le_resolutionX);


        hlayout_resolution->addLayout(hlayout_resolutionX);

        hlayout_resolutionY = new QHBoxLayout();
        hlayout_resolutionY->setSpacing(6);
        hlayout_resolutionY->setObjectName(QStringLiteral("hlayout_resolutionY"));
        lb_resolutionY = new QLabel(VoxelizeWidget);
        lb_resolutionY->setObjectName(QStringLiteral("lb_resolutionY"));

        hlayout_resolutionY->addWidget(lb_resolutionY);

        le_resolutionY = new QLineEdit(VoxelizeWidget);
        le_resolutionY->setObjectName(QStringLiteral("le_resolutionY"));
        le_resolutionY->setReadOnly(true);

        hlayout_resolutionY->addWidget(le_resolutionY);


        hlayout_resolution->addLayout(hlayout_resolutionY);

        hlayout_resolutionZ = new QHBoxLayout();
        hlayout_resolutionZ->setSpacing(6);
        hlayout_resolutionZ->setObjectName(QStringLiteral("hlayout_resolutionZ"));
        lb_resolutionZ = new QLabel(VoxelizeWidget);
        lb_resolutionZ->setObjectName(QStringLiteral("lb_resolutionZ"));

        hlayout_resolutionZ->addWidget(lb_resolutionZ);

        le_resolutionZ = new QLineEdit(VoxelizeWidget);
        le_resolutionZ->setObjectName(QStringLiteral("le_resolutionZ"));
        le_resolutionZ->setReadOnly(true);

        hlayout_resolutionZ->addWidget(le_resolutionZ);


        hlayout_resolution->addLayout(hlayout_resolutionZ);


        vlayout_widget->addLayout(hlayout_resolution);

        vlayout_solidModel = new QVBoxLayout();
        vlayout_solidModel->setSpacing(6);
        vlayout_solidModel->setObjectName(QStringLiteral("vlayout_solidModel"));
        lb_solidModel = new QLabel(VoxelizeWidget);
        lb_solidModel->setObjectName(QStringLiteral("lb_solidModel"));

        vlayout_solidModel->addWidget(lb_solidModel);

        hlayout_solidModel = new QHBoxLayout();
        hlayout_solidModel->setSpacing(6);
        hlayout_solidModel->setObjectName(QStringLiteral("hlayout_solidModel"));
        lb_solidModelSpace = new QLabel(VoxelizeWidget);
        lb_solidModelSpace->setObjectName(QStringLiteral("lb_solidModelSpace"));
        lb_solidModelSpace->setMaximumSize(QSize(10, 16777215));

        hlayout_solidModel->addWidget(lb_solidModelSpace);

        ckb_solidModel = new QCheckBox(VoxelizeWidget);
        ckb_solidModel->setObjectName(QStringLiteral("ckb_solidModel"));

        hlayout_solidModel->addWidget(ckb_solidModel);


        vlayout_solidModel->addLayout(hlayout_solidModel);


        vlayout_widget->addLayout(vlayout_solidModel);

        spr_widget = new QSpacerItem(20, 39, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vlayout_widget->addItem(spr_widget);

        btmbox_widget = new QDialogButtonBox(VoxelizeWidget);
        btmbox_widget->setObjectName(QStringLiteral("btmbox_widget"));
        btmbox_widget->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        vlayout_widget->addWidget(btmbox_widget);

        lb_voxelSize->raise();
        lb_resolution->raise();
        btmbox_widget->raise();
        titleBar->raise();

        retranslateUi(VoxelizeWidget);

        QMetaObject::connectSlotsByName(VoxelizeWidget);
    } // setupUi

    void retranslateUi(QWidget *VoxelizeWidget)
    {
        VoxelizeWidget->setWindowTitle(QApplication::translate("VoxelizeWidget", "VoxelizeWidget", Q_NULLPTR));
        lb_voxelSize->setText(QApplication::translate("VoxelizeWidget", "Voxel Size", Q_NULLPTR));
        lb_voxelSizeX->setText(QApplication::translate("VoxelizeWidget", "X:", Q_NULLPTR));
        le_voxelSizeX->setText(QApplication::translate("VoxelizeWidget", "0.4", Q_NULLPTR));
        lb_voxelSizeY->setText(QApplication::translate("VoxelizeWidget", "Y:", Q_NULLPTR));
        le_voxelSizeY->setText(QApplication::translate("VoxelizeWidget", "0.4", Q_NULLPTR));
        lb_voxelSizeZ->setText(QApplication::translate("VoxelizeWidget", "Z:", Q_NULLPTR));
        le_voxelSizeZ->setText(QApplication::translate("VoxelizeWidget", "0.4", Q_NULLPTR));
        lb_resolution->setText(QApplication::translate("VoxelizeWidget", "Resolution", Q_NULLPTR));
        lb_resolutionX->setText(QApplication::translate("VoxelizeWidget", "X:", Q_NULLPTR));
        lb_resolutionY->setText(QApplication::translate("VoxelizeWidget", "Y:", Q_NULLPTR));
        lb_resolutionZ->setText(QApplication::translate("VoxelizeWidget", "Z:", Q_NULLPTR));
        lb_solidModel->setText(QApplication::translate("VoxelizeWidget", "infill", Q_NULLPTR));
        lb_solidModelSpace->setText(QString());
        ckb_solidModel->setText(QApplication::translate("VoxelizeWidget", "Enable", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class VoxelizeWidget: public Ui_VoxelizeWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOXELIZEWIDGET_H
