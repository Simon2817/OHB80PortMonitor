/****************************************************************************
** Meta object code from reading C++ file 'firmware_upgrade_task.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../scheduler/tasks/firmware_upgrade_task.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firmware_upgrade_task.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirmwareUpgradeTask_t {
    QByteArrayData data[23];
    char stringdata0[295];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirmwareUpgradeTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirmwareUpgradeTask_t qt_meta_stringdata_FirmwareUpgradeTask = {
    {
QT_MOC_LITERAL(0, 0, 19), // "FirmwareUpgradeTask"
QT_MOC_LITERAL(1, 20, 14), // "deviceProgress"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 8), // "deviceId"
QT_MOC_LITERAL(4, 45, 7), // "percent"
QT_MOC_LITERAL(5, 53, 14), // "deviceStateLog"
QT_MOC_LITERAL(6, 68, 30), // "FirmwareUpgrader::UpgradeState"
QT_MOC_LITERAL(7, 99, 5), // "state"
QT_MOC_LITERAL(8, 105, 10), // "logMessage"
QT_MOC_LITERAL(9, 116, 5), // "frame"
QT_MOC_LITERAL(10, 122, 14), // "deviceFinished"
QT_MOC_LITERAL(11, 137, 7), // "success"
QT_MOC_LITERAL(12, 145, 7), // "message"
QT_MOC_LITERAL(13, 153, 11), // "allProgress"
QT_MOC_LITERAL(14, 165, 9), // "completed"
QT_MOC_LITERAL(15, 175, 5), // "total"
QT_MOC_LITERAL(16, 181, 21), // "onBinFileReadFinished"
QT_MOC_LITERAL(17, 203, 8), // "errorMsg"
QT_MOC_LITERAL(18, 212, 22), // "onUpgraderStateChanged"
QT_MOC_LITERAL(19, 235, 8), // "masterId"
QT_MOC_LITERAL(20, 244, 18), // "onUpgraderProgress"
QT_MOC_LITERAL(21, 263, 18), // "onUpgraderFinished"
QT_MOC_LITERAL(22, 282, 12) // "errorMessage"

    },
    "FirmwareUpgradeTask\0deviceProgress\0\0"
    "deviceId\0percent\0deviceStateLog\0"
    "FirmwareUpgrader::UpgradeState\0state\0"
    "logMessage\0frame\0deviceFinished\0success\0"
    "message\0allProgress\0completed\0total\0"
    "onBinFileReadFinished\0errorMsg\0"
    "onUpgraderStateChanged\0masterId\0"
    "onUpgraderProgress\0onUpgraderFinished\0"
    "errorMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirmwareUpgradeTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    4,   59,    2, 0x06 /* Public */,
      10,    3,   68,    2, 0x06 /* Public */,
      13,    2,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    2,   80,    2, 0x08 /* Private */,
      18,    4,   85,    2, 0x08 /* Private */,
      20,    2,   94,    2, 0x08 /* Private */,
      21,    4,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6, QMetaType::QString, QMetaType::QByteArray,    3,    7,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, QMetaType::QString,    3,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,   11,   17,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 6, QMetaType::QString, QMetaType::QByteArray,   19,    7,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   19,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool, 0x80000000 | 6, QMetaType::QString,   19,   11,    7,   22,

       0        // eod
};

void FirmwareUpgradeTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FirmwareUpgradeTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->deviceStateLog((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< FirmwareUpgrader::UpgradeState(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 2: _t->deviceFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 3: _t->allProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->onBinFileReadFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onUpgraderStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< FirmwareUpgrader::UpgradeState(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QByteArray(*)>(_a[4]))); break;
        case 6: _t->onUpgraderProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->onUpgraderFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< FirmwareUpgrader::UpgradeState(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FirmwareUpgradeTask::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeTask::deviceProgress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FirmwareUpgradeTask::*)(const QString & , FirmwareUpgrader::UpgradeState , const QString & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeTask::deviceStateLog)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FirmwareUpgradeTask::*)(const QString & , bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeTask::deviceFinished)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FirmwareUpgradeTask::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpgradeTask::allProgress)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FirmwareUpgradeTask::staticMetaObject = { {
    QMetaObject::SuperData::link<SchedulerTask::staticMetaObject>(),
    qt_meta_stringdata_FirmwareUpgradeTask.data,
    qt_meta_data_FirmwareUpgradeTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FirmwareUpgradeTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirmwareUpgradeTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirmwareUpgradeTask.stringdata0))
        return static_cast<void*>(this);
    return SchedulerTask::qt_metacast(_clname);
}

int FirmwareUpgradeTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SchedulerTask::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void FirmwareUpgradeTask::deviceProgress(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FirmwareUpgradeTask::deviceStateLog(const QString & _t1, FirmwareUpgrader::UpgradeState _t2, const QString & _t3, const QByteArray & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FirmwareUpgradeTask::deviceFinished(const QString & _t1, bool _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FirmwareUpgradeTask::allProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
