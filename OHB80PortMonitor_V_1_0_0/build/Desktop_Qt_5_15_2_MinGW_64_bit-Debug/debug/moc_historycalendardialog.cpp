/****************************************************************************
** Meta object code from reading C++ file 'historycalendardialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/loggerwidget/historycalendardialog/historycalendardialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'historycalendardialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HistoryCalendarDialog_t {
    QByteArrayData data[15];
    char stringdata0[191];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HistoryCalendarDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HistoryCalendarDialog_t qt_meta_stringdata_HistoryCalendarDialog = {
    {
QT_MOC_LITERAL(0, 0, 21), // "HistoryCalendarDialog"
QT_MOC_LITERAL(1, 22, 12), // "dateSelected"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "date"
QT_MOC_LITERAL(4, 41, 13), // "onYearChanged"
QT_MOC_LITERAL(5, 55, 5), // "index"
QT_MOC_LITERAL(6, 61, 14), // "onMonthChanged"
QT_MOC_LITERAL(7, 76, 21), // "onCalendarPageChanged"
QT_MOC_LITERAL(8, 98, 4), // "year"
QT_MOC_LITERAL(9, 103, 5), // "month"
QT_MOC_LITERAL(10, 109, 9), // "onConfirm"
QT_MOC_LITERAL(11, 119, 13), // "onDateClicked"
QT_MOC_LITERAL(12, 133, 20), // "updateCalendarFormat"
QT_MOC_LITERAL(13, 154, 17), // "populateYearCombo"
QT_MOC_LITERAL(14, 172, 18) // "populateMonthCombo"

    },
    "HistoryCalendarDialog\0dateSelected\0\0"
    "date\0onYearChanged\0index\0onMonthChanged\0"
    "onCalendarPageChanged\0year\0month\0"
    "onConfirm\0onDateClicked\0updateCalendarFormat\0"
    "populateYearCombo\0populateMonthCombo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HistoryCalendarDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   62,    2, 0x08 /* Private */,
       6,    1,   65,    2, 0x08 /* Private */,
       7,    2,   68,    2, 0x08 /* Private */,
      10,    0,   73,    2, 0x08 /* Private */,
      11,    1,   74,    2, 0x08 /* Private */,
      12,    0,   77,    2, 0x08 /* Private */,
      13,    0,   78,    2, 0x08 /* Private */,
      14,    0,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QDate,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HistoryCalendarDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HistoryCalendarDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dateSelected((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 1: _t->onYearChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onMonthChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onCalendarPageChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->onConfirm(); break;
        case 5: _t->onDateClicked((*reinterpret_cast< const QDate(*)>(_a[1]))); break;
        case 6: _t->updateCalendarFormat(); break;
        case 7: _t->populateYearCombo(); break;
        case 8: _t->populateMonthCombo(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (HistoryCalendarDialog::*)(const QDate & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HistoryCalendarDialog::dateSelected)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HistoryCalendarDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_HistoryCalendarDialog.data,
    qt_meta_data_HistoryCalendarDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HistoryCalendarDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HistoryCalendarDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HistoryCalendarDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int HistoryCalendarDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void HistoryCalendarDialog::dateSelected(const QDate & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
