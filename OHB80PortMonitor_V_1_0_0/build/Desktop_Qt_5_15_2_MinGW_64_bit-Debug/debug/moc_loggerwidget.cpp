/****************************************************************************
** Meta object code from reading C++ file 'loggerwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/loggerwidget/loggerwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSet>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'loggerwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LoggerWidget_t {
    QByteArrayData data[34];
    char stringdata0[426];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoggerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoggerWidget_t qt_meta_stringdata_LoggerWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "LoggerWidget"
QT_MOC_LITERAL(1, 13, 10), // "logWritten"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "success"
QT_MOC_LITERAL(4, 33, 11), // "onPageReady"
QT_MOC_LITERAL(5, 45, 4), // "Page"
QT_MOC_LITERAL(6, 50, 4), // "page"
QT_MOC_LITERAL(7, 55, 6), // "isPrev"
QT_MOC_LITERAL(8, 62, 14), // "onLoadProgress"
QT_MOC_LITERAL(9, 77, 7), // "percent"
QT_MOC_LITERAL(10, 85, 12), // "onLoadFailed"
QT_MOC_LITERAL(11, 98, 6), // "reason"
QT_MOC_LITERAL(12, 105, 20), // "onScrollValueChanged"
QT_MOC_LITERAL(13, 126, 5), // "value"
QT_MOC_LITERAL(14, 132, 20), // "onTopLoadMoreClicked"
QT_MOC_LITERAL(15, 153, 23), // "onBottomLoadMoreClicked"
QT_MOC_LITERAL(16, 177, 13), // "onLogAppended"
QT_MOC_LITERAL(17, 191, 11), // "pageChanged"
QT_MOC_LITERAL(18, 203, 19), // "onNavigationUpdated"
QT_MOC_LITERAL(19, 223, 7), // "hasPrev"
QT_MOC_LITERAL(20, 231, 7), // "hasNext"
QT_MOC_LITERAL(21, 239, 21), // "onJumpToLatestClicked"
QT_MOC_LITERAL(22, 261, 15), // "onSearchClicked"
QT_MOC_LITERAL(23, 277, 14), // "onHistoryReady"
QT_MOC_LITERAL(24, 292, 13), // "HistoryResult"
QT_MOC_LITERAL(25, 306, 6), // "result"
QT_MOC_LITERAL(26, 313, 20), // "onHistoryPageClicked"
QT_MOC_LITERAL(27, 334, 9), // "pageIndex"
QT_MOC_LITERAL(28, 344, 19), // "onSelectDateClicked"
QT_MOC_LITERAL(29, 364, 21), // "onAvailableDatesReady"
QT_MOC_LITERAL(30, 386, 11), // "QSet<QDate>"
QT_MOC_LITERAL(31, 398, 5), // "dates"
QT_MOC_LITERAL(32, 404, 10), // "onFindPrev"
QT_MOC_LITERAL(33, 415, 10) // "onFindNext"

    },
    "LoggerWidget\0logWritten\0\0success\0"
    "onPageReady\0Page\0page\0isPrev\0"
    "onLoadProgress\0percent\0onLoadFailed\0"
    "reason\0onScrollValueChanged\0value\0"
    "onTopLoadMoreClicked\0onBottomLoadMoreClicked\0"
    "onLogAppended\0pageChanged\0onNavigationUpdated\0"
    "hasPrev\0hasNext\0onJumpToLatestClicked\0"
    "onSearchClicked\0onHistoryReady\0"
    "HistoryResult\0result\0onHistoryPageClicked\0"
    "pageIndex\0onSelectDateClicked\0"
    "onAvailableDatesReady\0QSet<QDate>\0"
    "dates\0onFindPrev\0onFindNext"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoggerWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,  102,    2, 0x08 /* Private */,
       8,    1,  107,    2, 0x08 /* Private */,
      10,    1,  110,    2, 0x08 /* Private */,
      12,    1,  113,    2, 0x08 /* Private */,
      14,    0,  116,    2, 0x08 /* Private */,
      15,    0,  117,    2, 0x08 /* Private */,
      16,    2,  118,    2, 0x08 /* Private */,
      18,    2,  123,    2, 0x08 /* Private */,
      21,    0,  128,    2, 0x08 /* Private */,
      22,    0,  129,    2, 0x08 /* Private */,
      23,    1,  130,    2, 0x08 /* Private */,
      26,    1,  133,    2, 0x08 /* Private */,
      28,    0,  136,    2, 0x08 /* Private */,
      29,    1,  137,    2, 0x08 /* Private */,
      32,    0,  140,    2, 0x08 /* Private */,
      33,    0,  141,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::Bool,    6,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,    3,   17,
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool,   19,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 30,   31,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LoggerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LoggerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->logWritten((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->onPageReady((*reinterpret_cast< const Page(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->onLoadProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onLoadFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->onScrollValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->onTopLoadMoreClicked(); break;
        case 6: _t->onBottomLoadMoreClicked(); break;
        case 7: _t->onLogAppended((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->onNavigationUpdated((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->onJumpToLatestClicked(); break;
        case 10: _t->onSearchClicked(); break;
        case 11: _t->onHistoryReady((*reinterpret_cast< const HistoryResult(*)>(_a[1]))); break;
        case 12: _t->onHistoryPageClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->onSelectDateClicked(); break;
        case 14: _t->onAvailableDatesReady((*reinterpret_cast< const QSet<QDate>(*)>(_a[1]))); break;
        case 15: _t->onFindPrev(); break;
        case 16: _t->onFindNext(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Page >(); break;
            }
            break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< HistoryResult >(); break;
            }
            break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QSet<QDate> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LoggerWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LoggerWidget::logWritten)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LoggerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LoggerWidget.data,
    qt_meta_data_LoggerWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LoggerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoggerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LoggerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LoggerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void LoggerWidget::logWritten(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
