/****************************************************************************
** Meta object code from reading C++ file 'devicemonitorwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/overheadcranetrack/devicemonitorwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicemonitorwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Graph__DeviceMonitorWidget_t {
    QByteArrayData data[10];
    char stringdata0[168];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Graph__DeviceMonitorWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Graph__DeviceMonitorWidget_t qt_meta_stringdata_Graph__DeviceMonitorWidget = {
    {
QT_MOC_LITERAL(0, 0, 26), // "Graph::DeviceMonitorWidget"
QT_MOC_LITERAL(1, 27, 16), // "onDeviceSelected"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 23), // "FrameDevice::DeviceType"
QT_MOC_LITERAL(4, 69, 10), // "deviceType"
QT_MOC_LITERAL(5, 80, 28), // "QSharedPointer<SetOfOHBInfo>"
QT_MOC_LITERAL(6, 109, 7), // "setInfo"
QT_MOC_LITERAL(7, 117, 29), // "QSharedPointer<FoupOfOHBInfo>"
QT_MOC_LITERAL(8, 147, 8), // "foupInfo"
QT_MOC_LITERAL(9, 156, 11) // "refreshData"

    },
    "Graph::DeviceMonitorWidget\0onDeviceSelected\0"
    "\0FrameDevice::DeviceType\0deviceType\0"
    "QSharedPointer<SetOfOHBInfo>\0setInfo\0"
    "QSharedPointer<FoupOfOHBInfo>\0foupInfo\0"
    "refreshData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Graph__DeviceMonitorWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x08 /* Private */,
       9,    0,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7,    4,    6,    8,
    QMetaType::Void,

       0        // eod
};

void Graph::DeviceMonitorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DeviceMonitorWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDeviceSelected((*reinterpret_cast< FrameDevice::DeviceType(*)>(_a[1])),(*reinterpret_cast< QSharedPointer<SetOfOHBInfo>(*)>(_a[2])),(*reinterpret_cast< QSharedPointer<FoupOfOHBInfo>(*)>(_a[3]))); break;
        case 1: _t->refreshData(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Graph::DeviceMonitorWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Graph__DeviceMonitorWidget.data,
    qt_meta_data_Graph__DeviceMonitorWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Graph::DeviceMonitorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Graph::DeviceMonitorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Graph__DeviceMonitorWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Graph::DeviceMonitorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
