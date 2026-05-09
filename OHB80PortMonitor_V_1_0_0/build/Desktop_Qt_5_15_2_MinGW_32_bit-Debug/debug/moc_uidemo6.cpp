/****************************************************************************
** Meta object code from reading C++ file 'uidemo6.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/uidemo6.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uidemo6.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UIDemo6_t {
    QByteArrayData data[11];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UIDemo6_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UIDemo6_t qt_meta_stringdata_UIDemo6 = {
    {
QT_MOC_LITERAL(0, 0, 7), // "UIDemo6"
QT_MOC_LITERAL(1, 8, 15), // "requestMinimize"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 24), // "requestMaximizeOrRestore"
QT_MOC_LITERAL(4, 50, 12), // "requestClose"
QT_MOC_LITERAL(5, 63, 8), // "initForm"
QT_MOC_LITERAL(6, 72, 11), // "buttonClick"
QT_MOC_LITERAL(7, 84, 26), // "onScrollingTipLabelClicked"
QT_MOC_LITERAL(8, 111, 22), // "on_btnMenu_Min_clicked"
QT_MOC_LITERAL(9, 134, 22), // "on_btnMenu_Max_clicked"
QT_MOC_LITERAL(10, 157, 24) // "on_btnMenu_Close_clicked"

    },
    "UIDemo6\0requestMinimize\0\0"
    "requestMaximizeOrRestore\0requestClose\0"
    "initForm\0buttonClick\0onScrollingTipLabelClicked\0"
    "on_btnMenu_Min_clicked\0on_btnMenu_Max_clicked\0"
    "on_btnMenu_Close_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UIDemo6[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    0,   64,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UIDemo6::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UIDemo6 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestMinimize(); break;
        case 1: _t->requestMaximizeOrRestore(); break;
        case 2: _t->requestClose(); break;
        case 3: _t->initForm(); break;
        case 4: _t->buttonClick(); break;
        case 5: _t->onScrollingTipLabelClicked(); break;
        case 6: _t->on_btnMenu_Min_clicked(); break;
        case 7: _t->on_btnMenu_Max_clicked(); break;
        case 8: _t->on_btnMenu_Close_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UIDemo6::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UIDemo6::requestMinimize)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UIDemo6::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UIDemo6::requestMaximizeOrRestore)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UIDemo6::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UIDemo6::requestClose)) {
                *result = 2;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject UIDemo6::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_UIDemo6.data,
    qt_meta_data_UIDemo6,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UIDemo6::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UIDemo6::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UIDemo6.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int UIDemo6::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void UIDemo6::requestMinimize()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UIDemo6::requestMaximizeOrRestore()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void UIDemo6::requestClose()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
