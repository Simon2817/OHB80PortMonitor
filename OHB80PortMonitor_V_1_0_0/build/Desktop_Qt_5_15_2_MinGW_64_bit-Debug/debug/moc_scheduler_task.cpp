/****************************************************************************
** Meta object code from reading C++ file 'scheduler_task.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../scheduler/scheduler_task.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scheduler_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SchedulerTask_t {
    QByteArrayData data[27];
    char stringdata0[214];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SchedulerTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SchedulerTask_t qt_meta_stringdata_SchedulerTask = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SchedulerTask"
QT_MOC_LITERAL(1, 14, 12), // "stateChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 20), // "SchedulerTask::State"
QT_MOC_LITERAL(4, 49, 8), // "newState"
QT_MOC_LITERAL(5, 58, 8), // "progress"
QT_MOC_LITERAL(6, 67, 7), // "percent"
QT_MOC_LITERAL(7, 75, 3), // "msg"
QT_MOC_LITERAL(8, 79, 8), // "finished"
QT_MOC_LITERAL(9, 88, 7), // "success"
QT_MOC_LITERAL(10, 96, 10), // "dataResult"
QT_MOC_LITERAL(11, 107, 3), // "key"
QT_MOC_LITERAL(12, 111, 4), // "data"
QT_MOC_LITERAL(13, 116, 5), // "start"
QT_MOC_LITERAL(14, 122, 4), // "stop"
QT_MOC_LITERAL(15, 127, 5), // "State"
QT_MOC_LITERAL(16, 133, 7), // "Pending"
QT_MOC_LITERAL(17, 141, 7), // "Running"
QT_MOC_LITERAL(18, 149, 6), // "Paused"
QT_MOC_LITERAL(19, 156, 8), // "Finished"
QT_MOC_LITERAL(20, 165, 6), // "Failed"
QT_MOC_LITERAL(21, 172, 9), // "Cancelled"
QT_MOC_LITERAL(22, 182, 8), // "Priority"
QT_MOC_LITERAL(23, 191, 3), // "Low"
QT_MOC_LITERAL(24, 195, 6), // "Normal"
QT_MOC_LITERAL(25, 202, 4), // "High"
QT_MOC_LITERAL(26, 207, 6) // "Urgent"

    },
    "SchedulerTask\0stateChanged\0\0"
    "SchedulerTask::State\0newState\0progress\0"
    "percent\0msg\0finished\0success\0dataResult\0"
    "key\0data\0start\0stop\0State\0Pending\0"
    "Running\0Paused\0Finished\0Failed\0Cancelled\0"
    "Priority\0Low\0Normal\0High\0Urgent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SchedulerTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       2,   64, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    2,   47,    2, 0x06 /* Public */,
       8,    2,   52,    2, 0x06 /* Public */,
      10,    2,   57,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      13,    0,   62,    2, 0x02 /* Public */,
      14,    0,   63,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    9,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,   11,   12,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      15,   15, 0x0,    6,   74,
      22,   22, 0x0,    4,   86,

 // enum data: key, value
      16, uint(SchedulerTask::Pending),
      17, uint(SchedulerTask::Running),
      18, uint(SchedulerTask::Paused),
      19, uint(SchedulerTask::Finished),
      20, uint(SchedulerTask::Failed),
      21, uint(SchedulerTask::Cancelled),
      23, uint(SchedulerTask::Low),
      24, uint(SchedulerTask::Normal),
      25, uint(SchedulerTask::High),
      26, uint(SchedulerTask::Urgent),

       0        // eod
};

void SchedulerTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SchedulerTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stateChanged((*reinterpret_cast< SchedulerTask::State(*)>(_a[1]))); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->finished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->dataResult((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariantMap(*)>(_a[2]))); break;
        case 4: _t->start(); break;
        case 5: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< SchedulerTask::State >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SchedulerTask::*)(SchedulerTask::State );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SchedulerTask::stateChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SchedulerTask::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SchedulerTask::progress)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SchedulerTask::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SchedulerTask::finished)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SchedulerTask::*)(const QString & , const QVariantMap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SchedulerTask::dataResult)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SchedulerTask::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SchedulerTask.data,
    qt_meta_data_SchedulerTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SchedulerTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SchedulerTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SchedulerTask.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SchedulerTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SchedulerTask::stateChanged(SchedulerTask::State _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SchedulerTask::progress(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SchedulerTask::finished(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SchedulerTask::dataResult(const QString & _t1, const QVariantMap & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
