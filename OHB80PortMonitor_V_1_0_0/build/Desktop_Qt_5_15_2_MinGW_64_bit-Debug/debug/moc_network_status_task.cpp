/****************************************************************************
** Meta object code from reading C++ file 'network_status_task.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../scheduler/tasks/network_status_task.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'network_status_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkStatusTask_t {
    QByteArrayData data[14];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkStatusTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkStatusTask_t qt_meta_stringdata_NetworkStatusTask = {
    {
QT_MOC_LITERAL(0, 0, 17), // "NetworkStatusTask"
QT_MOC_LITERAL(1, 18, 15), // "allInitFinished"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "allSuccess"
QT_MOC_LITERAL(4, 46, 12), // "successCount"
QT_MOC_LITERAL(5, 59, 9), // "failCount"
QT_MOC_LITERAL(6, 69, 15), // "failedMasterIds"
QT_MOC_LITERAL(7, 85, 15), // "onStatusChanged"
QT_MOC_LITERAL(8, 101, 33), // "ModbusConnecter::ConnectionSt..."
QT_MOC_LITERAL(9, 135, 6), // "status"
QT_MOC_LITERAL(10, 142, 8), // "masterId"
QT_MOC_LITERAL(11, 151, 19), // "onInitCheckFinished"
QT_MOC_LITERAL(12, 171, 5), // "start"
QT_MOC_LITERAL(13, 177, 4) // "stop"

    },
    "NetworkStatusTask\0allInitFinished\0\0"
    "allSuccess\0successCount\0failCount\0"
    "failedMasterIds\0onStatusChanged\0"
    "ModbusConnecter::ConnectionStatus\0"
    "status\0masterId\0onInitCheckFinished\0"
    "start\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkStatusTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   48,    2, 0x08 /* Private */,
      11,    4,   53,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      12,    0,   62,    2, 0x02 /* Public */,
      13,    0,   63,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::Int, QMetaType::QStringList,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString,    9,   10,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::Int, QMetaType::QStringList,    3,    4,    5,    6,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetworkStatusTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkStatusTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->allInitFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4]))); break;
        case 1: _t->onStatusChanged((*reinterpret_cast< ModbusConnecter::ConnectionStatus(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->onInitCheckFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4]))); break;
        case 3: _t->start(); break;
        case 4: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusConnecter::ConnectionStatus >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkStatusTask::*)(bool , int , int , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkStatusTask::allInitFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkStatusTask::staticMetaObject = { {
    QMetaObject::SuperData::link<SchedulerTask::staticMetaObject>(),
    qt_meta_stringdata_NetworkStatusTask.data,
    qt_meta_data_NetworkStatusTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkStatusTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkStatusTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkStatusTask.stringdata0))
        return static_cast<void*>(this);
    return SchedulerTask::qt_metacast(_clname);
}

int NetworkStatusTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SchedulerTask::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NetworkStatusTask::allInitFinished(bool _t1, int _t2, int _t3, const QStringList & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
