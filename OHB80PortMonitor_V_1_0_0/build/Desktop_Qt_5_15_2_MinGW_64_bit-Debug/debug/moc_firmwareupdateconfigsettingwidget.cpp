/****************************************************************************
** Meta object code from reading C++ file 'firmwareupdateconfigsettingwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/debugsettingwidget/firmwareupdateconfigsettingwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'firmwareupdateconfigsettingwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FirmwareUpdateConfigSettingWidget_t {
    QByteArrayData data[10];
    char stringdata0[239];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FirmwareUpdateConfigSettingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FirmwareUpdateConfigSettingWidget_t qt_meta_stringdata_FirmwareUpdateConfigSettingWidget = {
    {
QT_MOC_LITERAL(0, 0, 33), // "FirmwareUpdateConfigSettingWi..."
QT_MOC_LITERAL(1, 34, 18), // "binFilePathChanged"
QT_MOC_LITERAL(2, 53, 0), // ""
QT_MOC_LITERAL(3, 54, 8), // "filePath"
QT_MOC_LITERAL(4, 63, 23), // "onLoadBinFileBtnClicked"
QT_MOC_LITERAL(5, 87, 29), // "onPrepareTimeoutSetBtnClicked"
QT_MOC_LITERAL(6, 117, 26), // "onWaitingTimeSetBtnClicked"
QT_MOC_LITERAL(7, 144, 27), // "onSendIntervalSetBtnClicked"
QT_MOC_LITERAL(8, 172, 30), // "onTransferTimeoutSetBtnClicked"
QT_MOC_LITERAL(9, 203, 35) // "onPostTransferWaitTimeSetBtnC..."

    },
    "FirmwareUpdateConfigSettingWidget\0"
    "binFilePathChanged\0\0filePath\0"
    "onLoadBinFileBtnClicked\0"
    "onPrepareTimeoutSetBtnClicked\0"
    "onWaitingTimeSetBtnClicked\0"
    "onSendIntervalSetBtnClicked\0"
    "onTransferTimeoutSetBtnClicked\0"
    "onPostTransferWaitTimeSetBtnClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FirmwareUpdateConfigSettingWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   52,    2, 0x08 /* Private */,
       5,    0,   53,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FirmwareUpdateConfigSettingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FirmwareUpdateConfigSettingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->binFilePathChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onLoadBinFileBtnClicked(); break;
        case 2: _t->onPrepareTimeoutSetBtnClicked(); break;
        case 3: _t->onWaitingTimeSetBtnClicked(); break;
        case 4: _t->onSendIntervalSetBtnClicked(); break;
        case 5: _t->onTransferTimeoutSetBtnClicked(); break;
        case 6: _t->onPostTransferWaitTimeSetBtnClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FirmwareUpdateConfigSettingWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FirmwareUpdateConfigSettingWidget::binFilePathChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FirmwareUpdateConfigSettingWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<SettingWidget::staticMetaObject>(),
    qt_meta_stringdata_FirmwareUpdateConfigSettingWidget.data,
    qt_meta_data_FirmwareUpdateConfigSettingWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FirmwareUpdateConfigSettingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FirmwareUpdateConfigSettingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FirmwareUpdateConfigSettingWidget.stringdata0))
        return static_cast<void*>(this);
    return SettingWidget::qt_metacast(_clname);
}

int FirmwareUpdateConfigSettingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SettingWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void FirmwareUpdateConfigSettingWidget::binFilePathChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
