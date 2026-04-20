/****************************************************************************
** Meta object code from reading C++ file 'logfilesystem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/loggerwidget/logfilesystem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#include <QtCore/QSet>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logfilesystem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LogFileSystem_t {
    QByteArrayData data[38];
    char stringdata0[461];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LogFileSystem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LogFileSystem_t qt_meta_stringdata_LogFileSystem = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LogFileSystem"
QT_MOC_LITERAL(1, 14, 9), // "pageReady"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "Page"
QT_MOC_LITERAL(4, 30, 4), // "page"
QT_MOC_LITERAL(5, 35, 6), // "isPrev"
QT_MOC_LITERAL(6, 42, 12), // "loadProgress"
QT_MOC_LITERAL(7, 55, 7), // "percent"
QT_MOC_LITERAL(8, 63, 10), // "loadFailed"
QT_MOC_LITERAL(9, 74, 6), // "reason"
QT_MOC_LITERAL(10, 81, 11), // "logAppended"
QT_MOC_LITERAL(11, 93, 7), // "success"
QT_MOC_LITERAL(12, 101, 11), // "pageChanged"
QT_MOC_LITERAL(13, 113, 22), // "navigationStateChanged"
QT_MOC_LITERAL(14, 136, 7), // "hasPrev"
QT_MOC_LITERAL(15, 144, 7), // "hasNext"
QT_MOC_LITERAL(16, 152, 11), // "currentFile"
QT_MOC_LITERAL(17, 164, 11), // "currentPage"
QT_MOC_LITERAL(18, 176, 9), // "pageCount"
QT_MOC_LITERAL(19, 186, 12), // "historyReady"
QT_MOC_LITERAL(20, 199, 13), // "HistoryResult"
QT_MOC_LITERAL(21, 213, 6), // "result"
QT_MOC_LITERAL(22, 220, 19), // "availableDatesReady"
QT_MOC_LITERAL(23, 240, 11), // "QSet<QDate>"
QT_MOC_LITERAL(24, 252, 5), // "dates"
QT_MOC_LITERAL(25, 258, 17), // "requestInitialize"
QT_MOC_LITERAL(26, 276, 15), // "requestPrevPage"
QT_MOC_LITERAL(27, 292, 15), // "requestNextPage"
QT_MOC_LITERAL(28, 308, 16), // "requestAppendLog"
QT_MOC_LITERAL(29, 325, 6), // "record"
QT_MOC_LITERAL(30, 332, 18), // "requestAppendBatch"
QT_MOC_LITERAL(31, 351, 20), // "QVector<QJsonObject>"
QT_MOC_LITERAL(32, 372, 7), // "records"
QT_MOC_LITERAL(33, 380, 19), // "requestCleanOldLogs"
QT_MOC_LITERAL(34, 400, 19), // "requestQueryHistory"
QT_MOC_LITERAL(35, 420, 12), // "HistoryQuery"
QT_MOC_LITERAL(36, 433, 5), // "query"
QT_MOC_LITERAL(37, 439, 21) // "requestAvailableDates"

    },
    "LogFileSystem\0pageReady\0\0Page\0page\0"
    "isPrev\0loadProgress\0percent\0loadFailed\0"
    "reason\0logAppended\0success\0pageChanged\0"
    "navigationStateChanged\0hasPrev\0hasNext\0"
    "currentFile\0currentPage\0pageCount\0"
    "historyReady\0HistoryResult\0result\0"
    "availableDatesReady\0QSet<QDate>\0dates\0"
    "requestInitialize\0requestPrevPage\0"
    "requestNextPage\0requestAppendLog\0"
    "record\0requestAppendBatch\0"
    "QVector<QJsonObject>\0records\0"
    "requestCleanOldLogs\0requestQueryHistory\0"
    "HistoryQuery\0query\0requestAvailableDates"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LogFileSystem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   89,    2, 0x06 /* Public */,
       6,    1,   94,    2, 0x06 /* Public */,
       8,    1,   97,    2, 0x06 /* Public */,
      10,    2,  100,    2, 0x06 /* Public */,
      13,    5,  105,    2, 0x06 /* Public */,
      19,    1,  116,    2, 0x06 /* Public */,
      22,    1,  119,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      25,    0,  122,    2, 0x0a /* Public */,
      26,    0,  123,    2, 0x0a /* Public */,
      27,    0,  124,    2, 0x0a /* Public */,
      28,    1,  125,    2, 0x0a /* Public */,
      30,    1,  128,    2, 0x0a /* Public */,
      33,    0,  131,    2, 0x0a /* Public */,
      34,    1,  132,    2, 0x0a /* Public */,
      37,    0,  135,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   11,   12,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::QString, QMetaType::Int, QMetaType::Int,   14,   15,   16,   17,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23,   24,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   29,
    QMetaType::Void, 0x80000000 | 31,   32,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 35,   36,
    QMetaType::Void,

       0        // eod
};

void LogFileSystem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LogFileSystem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pageReady((*reinterpret_cast< const Page(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->loadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->loadFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->logAppended((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->navigationStateChanged((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 5: _t->historyReady((*reinterpret_cast< const HistoryResult(*)>(_a[1]))); break;
        case 6: _t->availableDatesReady((*reinterpret_cast< const QSet<QDate>(*)>(_a[1]))); break;
        case 7: _t->requestInitialize(); break;
        case 8: _t->requestPrevPage(); break;
        case 9: _t->requestNextPage(); break;
        case 10: _t->requestAppendLog((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 11: _t->requestAppendBatch((*reinterpret_cast< const QVector<QJsonObject>(*)>(_a[1]))); break;
        case 12: _t->requestCleanOldLogs(); break;
        case 13: _t->requestQueryHistory((*reinterpret_cast< const HistoryQuery(*)>(_a[1]))); break;
        case 14: _t->requestAvailableDates(); break;
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
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QJsonObject> >(); break;
            }
            break;
        case 13:
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
            using _t = void (LogFileSystem::*)(const Page & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::pageReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LogFileSystem::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::loadProgress)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LogFileSystem::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::loadFailed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LogFileSystem::*)(bool , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::logAppended)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (LogFileSystem::*)(bool , bool , const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::navigationStateChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (LogFileSystem::*)(const HistoryResult & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::historyReady)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (LogFileSystem::*)(const QSet<QDate> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LogFileSystem::availableDatesReady)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LogFileSystem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LogFileSystem.data,
    qt_meta_data_LogFileSystem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LogFileSystem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LogFileSystem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LogFileSystem.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int LogFileSystem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void LogFileSystem::pageReady(const Page & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LogFileSystem::loadProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LogFileSystem::loadFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void LogFileSystem::logAppended(bool _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void LogFileSystem::navigationStateChanged(bool _t1, bool _t2, const QString & _t3, int _t4, int _t5)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t5))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void LogFileSystem::historyReady(const HistoryResult & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void LogFileSystem::availableDatesReady(const QSet<QDate> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
