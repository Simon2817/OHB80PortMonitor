/****************************************************************************
** Meta object code from reading C++ file 'init_check_task.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../scheduler/tasks/init_check_task.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'init_check_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InitCheckTask_t {
    QByteArrayData data[11];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InitCheckTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InitCheckTask_t qt_meta_stringdata_InitCheckTask = {
    {
QT_MOC_LITERAL(0, 0, 13), // "InitCheckTask"
QT_MOC_LITERAL(1, 14, 11), // "allFinished"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "allSuccess"
QT_MOC_LITERAL(4, 38, 12), // "successCount"
QT_MOC_LITERAL(5, 51, 9), // "failCount"
QT_MOC_LITERAL(6, 61, 15), // "failedMasterIds"
QT_MOC_LITERAL(7, 77, 17), // "onInitialFinished"
QT_MOC_LITERAL(8, 95, 20), // "QList<ModbusCommand>"
QT_MOC_LITERAL(9, 116, 14), // "failedCommands"
QT_MOC_LITERAL(10, 131, 8) // "masterId"

    },
    "InitCheckTask\0allFinished\0\0allSuccess\0"
    "successCount\0failCount\0failedMasterIds\0"
    "onInitialFinished\0QList<ModbusCommand>\0"
    "failedCommands\0masterId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InitCheckTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Int, QMetaType::Int, QMetaType::QStringList,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString,    9,   10,

       0        // eod
};

void InitCheckTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InitCheckTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->allFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4]))); break;
        case 1: _t->onInitialFinished((*reinterpret_cast< QList<ModbusCommand>(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<ModbusCommand> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InitCheckTask::*)(bool , int , int , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InitCheckTask::allFinished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InitCheckTask::staticMetaObject = { {
    QMetaObject::SuperData::link<SchedulerTask::staticMetaObject>(),
    qt_meta_stringdata_InitCheckTask.data,
    qt_meta_data_InitCheckTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *InitCheckTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InitCheckTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InitCheckTask.stringdata0))
        return static_cast<void*>(this);
    return SchedulerTask::qt_metacast(_clname);
}

int InitCheckTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SchedulerTask::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void InitCheckTask::allFinished(bool _t1, int _t2, int _t3, const QStringList & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
