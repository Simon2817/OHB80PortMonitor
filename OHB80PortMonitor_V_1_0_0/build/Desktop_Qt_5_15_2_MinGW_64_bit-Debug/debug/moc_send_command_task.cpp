/****************************************************************************
** Meta object code from reading C++ file 'send_command_task.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../scheduler/tasks/send_command_task.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'send_command_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SendCommandTask_t {
    QByteArrayData data[13];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SendCommandTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SendCommandTask_t qt_meta_stringdata_SendCommandTask = {
    {
QT_MOC_LITERAL(0, 0, 15), // "SendCommandTask"
QT_MOC_LITERAL(1, 16, 10), // "dataResult"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 6), // "qrcode"
QT_MOC_LITERAL(4, 35, 13), // "ModbusCommand"
QT_MOC_LITERAL(5, 49, 3), // "cmd"
QT_MOC_LITERAL(6, 53, 11), // "allFinished"
QT_MOC_LITERAL(7, 65, 10), // "allSuccess"
QT_MOC_LITERAL(8, 76, 12), // "successCount"
QT_MOC_LITERAL(9, 89, 9), // "failCount"
QT_MOC_LITERAL(10, 99, 9), // "failedIds"
QT_MOC_LITERAL(11, 109, 17), // "onCommandFinished"
QT_MOC_LITERAL(12, 127, 8) // "masterId"

    },
    "SendCommandTask\0dataResult\0\0qrcode\0"
    "ModbusCommand\0cmd\0allFinished\0allSuccess\0"
    "successCount\0failCount\0failedIds\0"
    "onCommandFinished\0masterId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SendCommandTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       6,    4,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    2,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::Int, QMetaType::QStringList,    7,    8,    9,   10,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::QString,    5,   12,

       0        // eod
};

void SendCommandTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SendCommandTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataResult((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const ModbusCommand(*)>(_a[2]))); break;
        case 1: _t->allFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4]))); break;
        case 2: _t->onCommandFinished((*reinterpret_cast< ModbusCommand(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusCommand >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusCommand >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SendCommandTask::*)(const QString & , const ModbusCommand & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SendCommandTask::dataResult)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SendCommandTask::*)(bool , int , int , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SendCommandTask::allFinished)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SendCommandTask::staticMetaObject = { {
    QMetaObject::SuperData::link<SchedulerTask::staticMetaObject>(),
    qt_meta_stringdata_SendCommandTask.data,
    qt_meta_data_SendCommandTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SendCommandTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SendCommandTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SendCommandTask.stringdata0))
        return static_cast<void*>(this);
    return SchedulerTask::qt_metacast(_clname);
}

int SendCommandTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void SendCommandTask::dataResult(const QString & _t1, const ModbusCommand & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SendCommandTask::allFinished(bool _t1, int _t2, int _t3, const QStringList & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
