/****************************************************************************
** Meta object code from reading C++ file 'logicalfilesystem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/loggerwidget/logicalfilesystem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSet>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logicalfilesystem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogicalFileSystem_t {
    QByteArrayData data[36];
    char stringdata0[443];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogicalFileSystem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogicalFileSystem_t qt_meta_stringdata_LogicalFileSystem = {
    {
QT_MOC_LITERAL(0, 0, 17), // "LogicalFileSystem"
QT_MOC_LITERAL(1, 18, 9), // "pageReady"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "Page"
QT_MOC_LITERAL(4, 34, 4), // "page"
QT_MOC_LITERAL(5, 39, 6), // "isPrev"
QT_MOC_LITERAL(6, 46, 12), // "loadProgress"
QT_MOC_LITERAL(7, 59, 7), // "percent"
QT_MOC_LITERAL(8, 67, 10), // "loadFailed"
QT_MOC_LITERAL(9, 78, 6), // "reason"
QT_MOC_LITERAL(10, 85, 11), // "logAppended"
QT_MOC_LITERAL(11, 97, 7), // "success"
QT_MOC_LITERAL(12, 105, 11), // "pageChanged"
QT_MOC_LITERAL(13, 117, 17), // "navigationUpdated"
QT_MOC_LITERAL(14, 135, 7), // "hasPrev"
QT_MOC_LITERAL(15, 143, 7), // "hasNext"
QT_MOC_LITERAL(16, 151, 12), // "historyReady"
QT_MOC_LITERAL(17, 164, 13), // "HistoryResult"
QT_MOC_LITERAL(18, 178, 6), // "result"
QT_MOC_LITERAL(19, 185, 19), // "availableDatesReady"
QT_MOC_LITERAL(20, 205, 11), // "QSet<QDate>"
QT_MOC_LITERAL(21, 217, 5), // "dates"
QT_MOC_LITERAL(22, 223, 18), // "_requestInitialize"
QT_MOC_LITERAL(23, 242, 16), // "_requestPrevPage"
QT_MOC_LITERAL(24, 259, 16), // "_requestNextPage"
QT_MOC_LITERAL(25, 276, 17), // "_requestAppendLog"
QT_MOC_LITERAL(26, 294, 6), // "record"
QT_MOC_LITERAL(27, 301, 20), // "_requestCleanOldLogs"
QT_MOC_LITERAL(28, 322, 20), // "_requestQueryHistory"
QT_MOC_LITERAL(29, 343, 12), // "HistoryQuery"
QT_MOC_LITERAL(30, 356, 5), // "query"
QT_MOC_LITERAL(31, 362, 22), // "_requestAvailableDates"
QT_MOC_LITERAL(32, 385, 24), // "onNavigationStateChanged"
QT_MOC_LITERAL(33, 410, 4), // "file"
QT_MOC_LITERAL(34, 415, 9), // "pageCount"
QT_MOC_LITERAL(35, 425, 17) // "onMidnightCleanup"

    },
    "LogicalFileSystem\0pageReady\0\0Page\0"
    "page\0isPrev\0loadProgress\0percent\0"
    "loadFailed\0reason\0logAppended\0success\0"
    "pageChanged\0navigationUpdated\0hasPrev\0"
    "hasNext\0historyReady\0HistoryResult\0"
    "result\0availableDatesReady\0QSet<QDate>\0"
    "dates\0_requestInitialize\0_requestPrevPage\0"
    "_requestNextPage\0_requestAppendLog\0"
    "record\0_requestCleanOldLogs\0"
    "_requestQueryHistory\0HistoryQuery\0"
    "query\0_requestAvailableDates\0"
    "onNavigationStateChanged\0file\0pageCount\0"
    "onMidnightCleanup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogicalFileSystem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   94,    2, 0x06 /* Public */,
       6,    1,   99,    2, 0x06 /* Public */,
       8,    1,  102,    2, 0x06 /* Public */,
      10,    2,  105,    2, 0x06 /* Public */,
      13,    2,  110,    2, 0x06 /* Public */,
      16,    1,  115,    2, 0x06 /* Public */,
      19,    1,  118,    2, 0x06 /* Public */,
      22,    0,  121,    2, 0x06 /* Public */,
      23,    0,  122,    2, 0x06 /* Public */,
      24,    0,  123,    2, 0x06 /* Public */,
      25,    1,  124,    2, 0x06 /* Public */,
      27,    0,  127,    2, 0x06 /* Public */,
      28,    1,  128,    2, 0x06 /* Public */,
      31,    0,  131,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      32,    5,  132,    2, 0x08 /* Private */,
      35,    0,  143,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   11,   12,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   14,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::QString, QMetaType::Int, QMetaType::Int,   14,   15,   33,    4,   34,
    QMetaType::Void,

       0        // eod
};

void LogicalFileSystem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogicalFileSystem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pageReady((*reinterpret_cast< const Page(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->loadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->loadFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->logAppended((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->navigationUpdated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->historyReady((*reinterpret_cast< const HistoryResult(*)>(_a[1]))); break;
        case 6: _t->availableDatesReady((*reinterpret_cast< const QSet<QDate>(*)>(_a[1]))); break;
        case 7: _t->_requestInitialize(); break;
        case 8: _t->_requestPrevPage(); break;
        case 9: _t->_requestNextPage(); break;
        case 10: _t->_requestAppendLog((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 11: _t->_requestCleanOldLogs(); break;
        case 12: _t->_requestQueryHistory((*reinterpret_cast< const HistoryQuery(*)>(_a[1]))); break;
        case 13: _t->_requestAvailableDates(); break;
        case 14: _t->onNavigationStateChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 15: _t->onMidnightCleanup(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Page >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< HistoryResult >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSet<QDate> >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< HistoryQuery >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LogicalFileSystem::*)(const Page & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::pageReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::loadProgress)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::loadFailed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::logAppended)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::navigationUpdated)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(const HistoryResult & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::historyReady)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(const QSet<QDate> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::availableDatesReady)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestInitialize)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestPrevPage)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestNextPage)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestAppendLog)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestCleanOldLogs)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)(const HistoryQuery & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestQueryHistory)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (LogicalFileSystem::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogicalFileSystem::_requestAvailableDates)) {
                *result = 13;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogicalFileSystem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LogicalFileSystem.data,
    qt_meta_data_LogicalFileSystem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogicalFileSystem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogicalFileSystem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogicalFileSystem.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LogicalFileSystem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void LogicalFileSystem::pageReady(const Page & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LogicalFileSystem::loadProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LogicalFileSystem::loadFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LogicalFileSystem::logAppended(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LogicalFileSystem::navigationUpdated(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LogicalFileSystem::historyReady(const HistoryResult & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void LogicalFileSystem::availableDatesReady(const QSet<QDate> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void LogicalFileSystem::_requestInitialize()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void LogicalFileSystem::_requestPrevPage()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void LogicalFileSystem::_requestNextPage()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void LogicalFileSystem::_requestAppendLog(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void LogicalFileSystem::_requestCleanOldLogs()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void LogicalFileSystem::_requestQueryHistory(const HistoryQuery & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void LogicalFileSystem::_requestAvailableDates()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
