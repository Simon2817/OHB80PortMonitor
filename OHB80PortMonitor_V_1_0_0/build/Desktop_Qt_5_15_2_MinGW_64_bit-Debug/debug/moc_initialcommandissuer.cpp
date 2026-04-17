/****************************************************************************
** Meta object code from reading C++ file 'initialcommandissuer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../data/modbustcpmastermanager/modbustcpmaster/initialcommandissuer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'initialcommandissuer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InitialCommandIssuer_t {
    QByteArrayData data[8];
    char stringdata0[104];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InitialCommandIssuer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InitialCommandIssuer_t qt_meta_stringdata_InitialCommandIssuer = {
    {
QT_MOC_LITERAL(0, 0, 20), // "InitialCommandIssuer"
QT_MOC_LITERAL(1, 21, 8), // "finished"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 20), // "QList<ModbusCommand>"
QT_MOC_LITERAL(4, 52, 14), // "failedCommands"
QT_MOC_LITERAL(5, 67, 15), // "onRoundComplete"
QT_MOC_LITERAL(6, 83, 12), // "onLogMessage"
QT_MOC_LITERAL(7, 96, 7) // "message"

    },
    "InitialCommandIssuer\0finished\0\0"
    "QList<ModbusCommand>\0failedCommands\0"
    "onRoundComplete\0onLogMessage\0message"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InitialCommandIssuer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   32,    2, 0x08 /* Private */,
       6,    1,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void InitialCommandIssuer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InitialCommandIssuer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< QList<ModbusCommand>(*)>(_a[1]))); break;
        case 1: _t->onRoundComplete((*reinterpret_cast< QList<ModbusCommand>(*)>(_a[1]))); break;
        case 2: _t->onLogMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InitialCommandIssuer::*)(QList<ModbusCommand> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InitialCommandIssuer::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InitialCommandIssuer::staticMetaObject = { {
    QMetaObject::SuperData::link<CyclicCommandIssuer::staticMetaObject>(),
    qt_meta_stringdata_InitialCommandIssuer.data,
    qt_meta_data_InitialCommandIssuer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *InitialCommandIssuer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InitialCommandIssuer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InitialCommandIssuer.stringdata0))
        return static_cast<void*>(this);
    return CyclicCommandIssuer::qt_metacast(_clname);
}

int InitialCommandIssuer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CyclicCommandIssuer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void InitialCommandIssuer::finished(QList<ModbusCommand> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
