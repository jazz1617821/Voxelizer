/****************************************************************************
** Meta object code from reading C++ file 'voxelizewidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../voxelizewidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'voxelizewidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VoxelizeWidget_t {
    QByteArrayData data[15];
    char stringdata0[150];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VoxelizeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VoxelizeWidget_t qt_meta_stringdata_VoxelizeWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VoxelizeWidget"
QT_MOC_LITERAL(1, 15, 14), // "sentVoxelModel"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "VoxelModel*"
QT_MOC_LITERAL(4, 43, 11), // "closeWidget"
QT_MOC_LITERAL(5, 55, 10), // "dragWidget"
QT_MOC_LITERAL(6, 66, 1), // "x"
QT_MOC_LITERAL(7, 68, 1), // "y"
QT_MOC_LITERAL(8, 70, 8), // "dragDone"
QT_MOC_LITERAL(9, 79, 7), // "confirm"
QT_MOC_LITERAL(10, 87, 6), // "cancel"
QT_MOC_LITERAL(11, 94, 15), // "changeVoxelSize"
QT_MOC_LITERAL(12, 110, 21), // "changeVoxelResolution"
QT_MOC_LITERAL(13, 132, 11), // "changeSolid"
QT_MOC_LITERAL(14, 144, 5) // "state"

    },
    "VoxelizeWidget\0sentVoxelModel\0\0"
    "VoxelModel*\0closeWidget\0dragWidget\0x\0"
    "y\0dragDone\0confirm\0cancel\0changeVoxelSize\0"
    "changeVoxelResolution\0changeSolid\0"
    "state"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VoxelizeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   62,    2, 0x08 /* Private */,
       5,    2,   63,    2, 0x08 /* Private */,
       8,    2,   68,    2, 0x08 /* Private */,
       9,    0,   73,    2, 0x08 /* Private */,
      10,    0,   74,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,
      12,    0,   76,    2, 0x08 /* Private */,
      13,    1,   77,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

void VoxelizeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        VoxelizeWidget *_t = static_cast<VoxelizeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sentVoxelModel((*reinterpret_cast< VoxelModel*(*)>(_a[1]))); break;
        case 1: _t->closeWidget(); break;
        case 2: _t->dragWidget((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->dragDone((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->confirm(); break;
        case 5: _t->cancel(); break;
        case 6: _t->changeVoxelSize(); break;
        case 7: _t->changeVoxelResolution(); break;
        case 8: _t->changeSolid((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (VoxelizeWidget::*_t)(VoxelModel * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VoxelizeWidget::sentVoxelModel)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VoxelizeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VoxelizeWidget.data,
      qt_meta_data_VoxelizeWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *VoxelizeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VoxelizeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VoxelizeWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int VoxelizeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void VoxelizeWidget::sentVoxelModel(VoxelModel * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
