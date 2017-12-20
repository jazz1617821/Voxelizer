/****************************************************************************
** Meta object code from reading C++ file 'imageprojectionwidget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../imageprojectionwidget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'imageprojectionwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextureWidget_t {
    QByteArrayData data[5];
    char stringdata0[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextureWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextureWidget_t qt_meta_stringdata_TextureWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "TextureWidget"
QT_MOC_LITERAL(1, 14, 12), // "imageUpdated"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "QImage*"
QT_MOC_LITERAL(4, 36, 5) // "image"

    },
    "TextureWidget\0imageUpdated\0\0QImage*\0"
    "image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void TextureWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TextureWidget *_t = static_cast<TextureWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageUpdated((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TextureWidget::*_t)(QImage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureWidget::imageUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TextureWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TextureWidget.data,
      qt_meta_data_TextureWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TextureWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TextureWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void TextureWidget::imageUpdated(QImage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_TextureMappingWidget_t {
    QByteArrayData data[7];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextureMappingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextureMappingWidget_t qt_meta_stringdata_TextureMappingWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "TextureMappingWidget"
QT_MOC_LITERAL(1, 21, 14), // "textureUpdated"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 7), // "QImage*"
QT_MOC_LITERAL(4, 45, 7), // "texture"
QT_MOC_LITERAL(5, 53, 12), // "imageUpdated"
QT_MOC_LITERAL(6, 66, 5) // "image"

    },
    "TextureMappingWidget\0textureUpdated\0"
    "\0QImage*\0texture\0imageUpdated\0image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextureMappingWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    6,

       0        // eod
};

void TextureMappingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TextureMappingWidget *_t = static_cast<TextureMappingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textureUpdated((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 1: _t->imageUpdated((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (TextureMappingWidget::*_t)(QImage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TextureMappingWidget::textureUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TextureMappingWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TextureMappingWidget.data,
      qt_meta_data_TextureMappingWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TextureMappingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextureMappingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextureMappingWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TextureMappingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void TextureMappingWidget::textureUpdated(QImage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ImageProjectionWidget_t {
    QByteArrayData data[12];
    char stringdata0[110];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageProjectionWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageProjectionWidget_t qt_meta_stringdata_ImageProjectionWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "ImageProjectionWidget"
QT_MOC_LITERAL(1, 22, 11), // "sendTexture"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 7), // "QImage*"
QT_MOC_LITERAL(4, 43, 7), // "texture"
QT_MOC_LITERAL(5, 51, 8), // "openFile"
QT_MOC_LITERAL(6, 60, 11), // "closeWidget"
QT_MOC_LITERAL(7, 72, 7), // "dragged"
QT_MOC_LITERAL(8, 80, 1), // "x"
QT_MOC_LITERAL(9, 82, 1), // "y"
QT_MOC_LITERAL(10, 84, 14), // "textureUpdated"
QT_MOC_LITERAL(11, 99, 10) // "resetImage"

    },
    "ImageProjectionWidget\0sendTexture\0\0"
    "QImage*\0texture\0openFile\0closeWidget\0"
    "dragged\0x\0y\0textureUpdated\0resetImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageProjectionWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    2,   49,    2, 0x0a /* Public */,
      10,    1,   54,    2, 0x0a /* Public */,
      11,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void ImageProjectionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ImageProjectionWidget *_t = static_cast<ImageProjectionWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendTexture((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 1: _t->openFile(); break;
        case 2: _t->closeWidget(); break;
        case 3: _t->dragged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->textureUpdated((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 5: _t->resetImage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ImageProjectionWidget::*_t)(QImage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProjectionWidget::sendTexture)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject ImageProjectionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ImageProjectionWidget.data,
      qt_meta_data_ImageProjectionWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ImageProjectionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageProjectionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageProjectionWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ImageProjectionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ImageProjectionWidget::sendTexture(QImage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
