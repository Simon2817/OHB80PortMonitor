/****************************************************************************
** Meta object code from reading C++ file 'modbustcpmaster.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../data/modbustcpmastermanager/modbustcpmaster/modbustcpmaster.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'modbustcpmaster.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ModbusTcpMaster_t {
    QByteArrayData data[24];
    char stringdata0[338];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ModbusTcpMaster_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ModbusTcpMaster_t qt_meta_stringdata_ModbusTcpMaster = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ModbusTcpMaster"
QT_MOC_LITERAL(1, 16, 13), // "errorOccurred"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 22), // "ModbusTcpMaster::State"
QT_MOC_LITERAL(4, 54, 5), // "state"
QT_MOC_LITERAL(5, 60, 7), // "message"
QT_MOC_LITERAL(6, 68, 12), // "stateChanged"
QT_MOC_LITERAL(7, 81, 25), // "onConnectionStatusChanged"
QT_MOC_LITERAL(8, 107, 33), // "ModbusConnecter::ConnectionSt..."
QT_MOC_LITERAL(9, 141, 6), // "status"
QT_MOC_LITERAL(10, 148, 8), // "masterId"
QT_MOC_LITERAL(11, 157, 17), // "onConnectionError"
QT_MOC_LITERAL(12, 175, 17), // "onInitialFinished"
QT_MOC_LITERAL(13, 193, 20), // "QList<ModbusCommand>"
QT_MOC_LITERAL(14, 214, 14), // "failedCommands"
QT_MOC_LITERAL(15, 229, 29), // "onPeriodicDisconnectRequested"
QT_MOC_LITERAL(16, 259, 5), // "State"
QT_MOC_LITERAL(17, 265, 4), // "Idle"
QT_MOC_LITERAL(18, 270, 10), // "Connecting"
QT_MOC_LITERAL(19, 281, 13), // "SenderStartup"
QT_MOC_LITERAL(20, 295, 12), // "Initializing"
QT_MOC_LITERAL(21, 308, 15), // "PeriodicStartup"
QT_MOC_LITERAL(22, 324, 7), // "Running"
QT_MOC_LITERAL(23, 332, 5) // "Error"

    },
    "ModbusTcpMaster\0errorOccurred\0\0"
    "ModbusTcpMaster::State\0state\0message\0"
    "stateChanged\0onConnectionStatusChanged\0"
    "ModbusConnecter::ConnectionStatus\0"
    "status\0masterId\0onConnectionError\0"
    "onInitialFinished\0QList<ModbusCommand>\0"
    "failedCommands\0onPeriodicDisconnectRequested\0"
    "State\0Idle\0Connecting\0SenderStartup\0"
    "Initializing\0PeriodicStartup\0Running\0"
    "Error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ModbusTcpMaster[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       1,   64, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       6,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   52,    2, 0x08 /* Private */,
      11,    1,   57,    2, 0x08 /* Private */,
      12,    1,   60,    2, 0x08 /* Private */,
      15,    0,   63,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 8, QMetaType::QString,    9,   10,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
      16,   16, 0x2,    7,   69,

 // enum data: key, value
      17, uint(ModbusTcpMaster::State::Idle),
      18, uint(ModbusTcpMaster::State::Connecting),
      19, uint(ModbusTcpMaster::State::SenderStartup),
      20, uint(ModbusTcpMaster::State::Initializing),
      21, uint(ModbusTcpMaster::State::PeriodicStartup),
      22, uint(ModbusTcpMaster::State::Running),
      23, uint(ModbusTcpMaster::State::Error),

       0        // eod
};

void ModbusTcpMaster::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ModbusTcpMaster *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorOccurred((*reinterpret_cast< ModbusTcpMaster::State(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->stateChanged((*reinterpret_cast< ModbusTcpMaster::State(*)>(_a[1]))); break;
        case 2: _t->onConnectionStatusChanged((*reinterpret_cast< ModbusConnecter::ConnectionStatus(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->onConnectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onInitialFinished((*reinterpret_cast< QList<ModbusCommand>(*)>(_a[1]))); break;
        case 5: _t->onPeriodicDisconnectRequested(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< ModbusConnecter::ConnectionStatus >(); break;
            }
            break;
        case 4:
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
            using _t = void (ModbusTcpMaster::*)(ModbusTcpMaster::State , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusTcpMaster::errorOccurred)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ModbusTcpMaster::*)(ModbusTcpMaster::State );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ModbusTcpMaster::stateChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ModbusTcpMaster::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ModbusTcpMaster.data,
    qt_meta_data_ModbusTcpMaster,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ModbusTcpMaster::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ModbusTcpMaster::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ModbusTcpMaster.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ModbusTcpMaster::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ModbusTcpMaster::errorOccurred(ModbusTcpMaster::State _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ModbusTcpMaster::stateChanged(ModbusTcpMaster::State _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
