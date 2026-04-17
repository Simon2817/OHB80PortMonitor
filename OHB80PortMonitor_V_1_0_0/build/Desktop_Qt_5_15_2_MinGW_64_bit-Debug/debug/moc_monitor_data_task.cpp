/****************************************************************************
** Meta object code from reading C++ file 'monitor_data_task.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../scheduler/tasks/monitor_data_task.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'monitor_data_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MonitorDataTask_t {
    QByteArrayData data[8];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MonitorDataTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MonitorDataTask_t qt_meta_stringdata_MonitorDataTask = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MonitorDataTask"
QT_MOC_LITERAL(1, 16, 18), // "onCommandCompleted"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 13), // "ModbusCommand"
QT_MOC_LITERAL(4, 50, 3), // "cmd"
QT_MOC_LITERAL(5, 54, 8), // "masterId"
QT_MOC_LITERAL(6, 63, 5), // "start"
QT_MOC_LITERAL(7, 69, 4) // "stop"

    },
    "MonitorDataTask\0onCommandCompleted\0\0"
    "ModbusCommand\0cmd\0masterId\0start\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MonitorDataTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
       6,    0,   34,    2, 0x02 /* Public */,
       7,    0,   35,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MonitorDataTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MonitorDataTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onCommandCompleted((*reinterpret_cast< ModbusCommand(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->start(); break;
        case 2: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusCommand >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MonitorDataTask::staticMetaObject = { {
    QMetaObject::SuperData::link<SchedulerTask::staticMetaObject>(),
    qt_meta_stringdata_MonitorDataTask.data,
    qt_meta_data_MonitorDataTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MonitorDataTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MonitorDataTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MonitorDataTask.stringdata0))
        return static_cast<void*>(this);
    return SchedulerTask::qt_metacast(_clname);
}

int MonitorDataTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SchedulerTask::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
