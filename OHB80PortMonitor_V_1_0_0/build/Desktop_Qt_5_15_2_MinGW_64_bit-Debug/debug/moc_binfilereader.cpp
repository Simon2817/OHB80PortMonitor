/****************************************************************************
** Meta object code from reading C++ file 'binfilereader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../tool/binfilereader/binfilereader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'binfilereader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BinFileReader_t {
    QByteArrayData data[12];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BinFileReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BinFileReader_t qt_meta_stringdata_BinFileReader = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BinFileReader"
QT_MOC_LITERAL(1, 14, 15), // "sigReadProgress"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 7), // "percent"
QT_MOC_LITERAL(4, 39, 15), // "sigReadFinished"
QT_MOC_LITERAL(5, 55, 7), // "success"
QT_MOC_LITERAL(6, 63, 8), // "errorMsg"
QT_MOC_LITERAL(7, 72, 9), // "ReadState"
QT_MOC_LITERAL(8, 82, 7), // "NotRead"
QT_MOC_LITERAL(9, 90, 7), // "Reading"
QT_MOC_LITERAL(10, 98, 12), // "ReadComplete"
QT_MOC_LITERAL(11, 111, 10) // "ReadFailed"

    },
    "BinFileReader\0sigReadProgress\0\0percent\0"
    "sigReadFinished\0success\0errorMsg\0"
    "ReadState\0NotRead\0Reading\0ReadComplete\0"
    "ReadFailed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BinFileReader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       1,   32, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    2,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool, QMetaType::QString,    5,    6,

 // enums: name, alias, flags, count, data
       7,    7, 0x0,    4,   37,

 // enum data: key, value
       8, uint(BinFileReader::NotRead),
       9, uint(BinFileReader::Reading),
      10, uint(BinFileReader::ReadComplete),
      11, uint(BinFileReader::ReadFailed),

       0        // eod
};

void BinFileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BinFileReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigReadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->sigReadFinished((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BinFileReader::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BinFileReader::sigReadProgress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BinFileReader::*)(bool , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BinFileReader::sigReadFinished)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BinFileReader::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_BinFileReader.data,
    qt_meta_data_BinFileReader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BinFileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BinFileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BinFileReader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BinFileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void BinFileReader::sigReadProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BinFileReader::sigReadFinished(bool _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
