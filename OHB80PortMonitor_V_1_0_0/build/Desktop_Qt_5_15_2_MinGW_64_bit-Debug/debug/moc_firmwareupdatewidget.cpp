/****************************************************************************
** Meta object code from reading C++ file 'firmwareupdatewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/debugsettingwidget/firmwareupdatewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firmwareupdatewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirmwareUpdateWidget_t {
    QByteArrayData data[20];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirmwareUpdateWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirmwareUpdateWidget_t qt_meta_stringdata_FirmwareUpdateWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "FirmwareUpdateWidget"
QT_MOC_LITERAL(1, 21, 20), // "onAddDeviceForUpdate"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 15), // "onAddAllDevices"
QT_MOC_LITERAL(4, 59, 7), // "onClear"
QT_MOC_LITERAL(5, 67, 23), // "onUpdateSelectedDevices"
QT_MOC_LITERAL(6, 91, 20), // "onTaskDeviceProgress"
QT_MOC_LITERAL(7, 112, 6), // "qrcode"
QT_MOC_LITERAL(8, 119, 7), // "percent"
QT_MOC_LITERAL(9, 127, 20), // "onTaskDeviceStateLog"
QT_MOC_LITERAL(10, 148, 30), // "FirmwareUpgrader::UpgradeState"
QT_MOC_LITERAL(11, 179, 5), // "state"
QT_MOC_LITERAL(12, 185, 10), // "logMessage"
QT_MOC_LITERAL(13, 196, 5), // "frame"
QT_MOC_LITERAL(14, 202, 20), // "onTaskDeviceFinished"
QT_MOC_LITERAL(15, 223, 7), // "success"
QT_MOC_LITERAL(16, 231, 7), // "message"
QT_MOC_LITERAL(17, 239, 17), // "onTaskAllProgress"
QT_MOC_LITERAL(18, 257, 9), // "completed"
QT_MOC_LITERAL(19, 267, 5) // "total"

    },
    "FirmwareUpdateWidget\0onAddDeviceForUpdate\0"
    "\0onAddAllDevices\0onClear\0"
    "onUpdateSelectedDevices\0onTaskDeviceProgress\0"
    "qrcode\0percent\0onTaskDeviceStateLog\0"
    "FirmwareUpgrader::UpgradeState\0state\0"
    "logMessage\0frame\0onTaskDeviceFinished\0"
    "success\0message\0onTaskAllProgress\0"
    "completed\0total"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirmwareUpdateWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    2,   58,    2, 0x08 /* Private */,
       9,    4,   63,    2, 0x08 /* Private */,
      14,    3,   72,    2, 0x08 /* Private */,
      17,    2,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, QMetaType::QString, QMetaType::QByteArray,    7,   11,   12,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,    7,   15,   16,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   18,   19,

       0        // eod
};

void FirmwareUpdateWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FirmwareUpdateWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onAddDeviceForUpdate(); break;
        case 1: _t->onAddAllDevices(); break;
        case 2: _t->onClear(); break;
        case 3: _t->onUpdateSelectedDevices(); break;
        case 4: _t->onTaskDeviceProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->onTaskDeviceStateLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< FirmwareUpgrader::UpgradeState(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 6: _t->onTaskDeviceFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 7: _t->onTaskAllProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FirmwareUpdateWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_FirmwareUpdateWidget.data,
    qt_meta_data_FirmwareUpdateWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FirmwareUpdateWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirmwareUpdateWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirmwareUpdateWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FirmwareUpdateWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
