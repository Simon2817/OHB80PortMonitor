/****************************************************************************
** Meta object code from reading C++ file 'periodiccommandsender.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../data/modbustcpmastermanager/modbustcpmaster/periodiccommandsender.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'periodiccommandsender.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PeriodicCommandSender_t {
    QByteArrayData data[11];
    char stringdata0[148];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PeriodicCommandSender_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PeriodicCommandSender_t qt_meta_stringdata_PeriodicCommandSender = {
    {
QT_MOC_LITERAL(0, 0, 21), // "PeriodicCommandSender"
QT_MOC_LITERAL(1, 22, 16), // "commandCompleted"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 13), // "ModbusCommand"
QT_MOC_LITERAL(4, 54, 3), // "cmd"
QT_MOC_LITERAL(5, 58, 16), // "disconnectDevice"
QT_MOC_LITERAL(6, 75, 15), // "onRoundComplete"
QT_MOC_LITERAL(7, 91, 20), // "QList<ModbusCommand>"
QT_MOC_LITERAL(8, 112, 14), // "failedCommands"
QT_MOC_LITERAL(9, 127, 12), // "onLogMessage"
QT_MOC_LITERAL(10, 140, 7) // "message"

    },
    "PeriodicCommandSender\0commandCompleted\0"
    "\0ModbusCommand\0cmd\0disconnectDevice\0"
    "onRoundComplete\0QList<ModbusCommand>\0"
    "failedCommands\0onLogMessage\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PeriodicCommandSender[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    0,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   38,    2, 0x08 /* Private */,
       9,    1,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void PeriodicCommandSender::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PeriodicCommandSender *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commandCompleted((*reinterpret_cast< ModbusCommand(*)>(_a[1]))); break;
        case 1: _t->disconnectDevice(); break;
        case 2: _t->onRoundComplete((*reinterpret_cast< QList<ModbusCommand>(*)>(_a[1]))); break;
        case 3: _t->onLogMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PeriodicCommandSender::*)(ModbusCommand );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PeriodicCommandSender::commandCompleted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PeriodicCommandSender::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PeriodicCommandSender::disconnectDevice)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PeriodicCommandSender::staticMetaObject = { {
    QMetaObject::SuperData::link<CyclicCommandIssuer::staticMetaObject>(),
    qt_meta_stringdata_PeriodicCommandSender.data,
    qt_meta_data_PeriodicCommandSender,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PeriodicCommandSender::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PeriodicCommandSender::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PeriodicCommandSender.stringdata0))
        return static_cast<void*>(this);
    return CyclicCommandIssuer::qt_metacast(_clname);
}

int PeriodicCommandSender::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CyclicCommandIssuer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PeriodicCommandSender::commandCompleted(ModbusCommand _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PeriodicCommandSender::disconnectDevice()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
