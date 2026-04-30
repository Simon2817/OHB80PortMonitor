/****************************************************************************
** Meta object code from reading C++ file 'modbuscommandsender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../data/modbustcpmastermanager/modbustcpmaster/modbuscommandsender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modbuscommandsender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModbusCommandSender_t {
    QByteArrayData data[12];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModbusCommandSender_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModbusCommandSender_t qt_meta_stringdata_ModbusCommandSender = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ModbusCommandSender"
QT_MOC_LITERAL(1, 20, 15), // "commandFinished"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "ModbusCommand"
QT_MOC_LITERAL(4, 51, 3), // "cmd"
QT_MOC_LITERAL(5, 55, 8), // "masterId"
QT_MOC_LITERAL(6, 64, 19), // "commandTimeoutRetry"
QT_MOC_LITERAL(7, 84, 8), // "dispatch"
QT_MOC_LITERAL(8, 93, 19), // "onReceiverSucceeded"
QT_MOC_LITERAL(9, 113, 16), // "onReceiverFailed"
QT_MOC_LITERAL(10, 130, 8), // "timedOut"
QT_MOC_LITERAL(11, 139, 13) // "checksumError"

    },
    "ModbusCommandSender\0commandFinished\0"
    "\0ModbusCommand\0cmd\0masterId\0"
    "commandTimeoutRetry\0dispatch\0"
    "onReceiverSucceeded\0onReceiverFailed\0"
    "timedOut\0checksumError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModbusCommandSender[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       6,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   49,    2, 0x08 /* Private */,
       8,    1,   50,    2, 0x08 /* Private */,
       9,    3,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool, QMetaType::Bool,    4,   10,   11,

       0        // eod
};

void ModbusCommandSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModbusCommandSender *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commandFinished((*reinterpret_cast< ModbusCommand(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->commandTimeoutRetry((*reinterpret_cast< ModbusCommand(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->dispatch(); break;
        case 3: _t->onReceiverSucceeded((*reinterpret_cast< ModbusCommand(*)>(_a[1]))); break;
        case 4: _t->onReceiverFailed((*reinterpret_cast< ModbusCommand(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
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
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusCommand >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusCommand >(); break;
            }
            break;
        case 4:
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
            using _t = void (ModbusCommandSender::*)(ModbusCommand , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusCommandSender::commandFinished)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModbusCommandSender::*)(ModbusCommand , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusCommandSender::commandTimeoutRetry)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModbusCommandSender::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ModbusCommandSender.data,
    qt_meta_data_ModbusCommandSender,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModbusCommandSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModbusCommandSender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModbusCommandSender.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ModbusCommandSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void ModbusCommandSender::commandFinished(ModbusCommand _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ModbusCommandSender::commandTimeoutRetry(ModbusCommand _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
