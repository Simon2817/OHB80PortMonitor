/****************************************************************************
** Meta object code from reading C++ file 'cranemapwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ui/customwidget/overheadcranetrack/cranemapwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cranemapwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CraneMapWidget_t {
    QByteArrayData data[22];
    char stringdata0[349];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CraneMapWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CraneMapWidget_t qt_meta_stringdata_CraneMapWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CraneMapWidget"
QT_MOC_LITERAL(1, 15, 14), // "deviceSelected"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 23), // "FrameDevice::DeviceType"
QT_MOC_LITERAL(4, 55, 10), // "deviceType"
QT_MOC_LITERAL(5, 66, 28), // "QSharedPointer<SetOfOHBInfo>"
QT_MOC_LITERAL(6, 95, 7), // "setInfo"
QT_MOC_LITERAL(7, 103, 29), // "QSharedPointer<FoupOfOHBInfo>"
QT_MOC_LITERAL(8, 133, 8), // "foupInfo"
QT_MOC_LITERAL(9, 142, 17), // "configLoadStarted"
QT_MOC_LITERAL(10, 160, 18), // "configLoadProgress"
QT_MOC_LITERAL(11, 179, 10), // "percentage"
QT_MOC_LITERAL(12, 190, 7), // "message"
QT_MOC_LITERAL(13, 198, 18), // "configLoadFinished"
QT_MOC_LITERAL(14, 217, 7), // "success"
QT_MOC_LITERAL(15, 225, 17), // "refreshDeviceData"
QT_MOC_LITERAL(16, 243, 14), // "selectFirstSet"
QT_MOC_LITERAL(17, 258, 28), // "handleSetDeviceDoubleClicked"
QT_MOC_LITERAL(18, 287, 4), // "uiId"
QT_MOC_LITERAL(19, 292, 15), // "firstFoupQrCode"
QT_MOC_LITERAL(20, 308, 19), // "handleDeviceClicked"
QT_MOC_LITERAL(21, 328, 20) // "onConfigLoadFinished"

    },
    "CraneMapWidget\0deviceSelected\0\0"
    "FrameDevice::DeviceType\0deviceType\0"
    "QSharedPointer<SetOfOHBInfo>\0setInfo\0"
    "QSharedPointer<FoupOfOHBInfo>\0foupInfo\0"
    "configLoadStarted\0configLoadProgress\0"
    "percentage\0message\0configLoadFinished\0"
    "success\0refreshDeviceData\0selectFirstSet\0"
    "handleSetDeviceDoubleClicked\0uiId\0"
    "firstFoupQrCode\0handleDeviceClicked\0"
    "onConfigLoadFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CraneMapWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   59,    2, 0x06 /* Public */,
       9,    0,   66,    2, 0x06 /* Public */,
      10,    2,   67,    2, 0x06 /* Public */,
      13,    1,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   75,    2, 0x0a /* Public */,
      16,    0,   76,    2, 0x0a /* Public */,
      17,    2,   77,    2, 0x08 /* Private */,
      20,    2,   82,    2, 0x08 /* Private */,
      21,    0,   87,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5, 0x80000000 | 7,    4,    6,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   11,   12,
    QMetaType::Void, QMetaType::Bool,   14,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   18,   19,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,   18,
    QMetaType::Void,

       0        // eod
};

void CraneMapWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CraneMapWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceSelected((*reinterpret_cast< FrameDevice::DeviceType(*)>(_a[1])),(*reinterpret_cast< QSharedPointer<SetOfOHBInfo>(*)>(_a[2])),(*reinterpret_cast< QSharedPointer<FoupOfOHBInfo>(*)>(_a[3]))); break;
        case 1: _t->configLoadStarted(); break;
        case 2: _t->configLoadProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->configLoadFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->refreshDeviceData(); break;
        case 5: _t->selectFirstSet(); break;
        case 6: _t->handleSetDeviceDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->handleDeviceClicked((*reinterpret_cast< FrameDevice::DeviceType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->onConfigLoadFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CraneMapWidget::*)(FrameDevice::DeviceType , QSharedPointer<SetOfOHBInfo> , QSharedPointer<FoupOfOHBInfo> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CraneMapWidget::deviceSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CraneMapWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CraneMapWidget::configLoadStarted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CraneMapWidget::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CraneMapWidget::configLoadProgress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CraneMapWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CraneMapWidget::configLoadFinished)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CraneMapWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CraneMapWidget.data,
    qt_meta_data_CraneMapWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CraneMapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CraneMapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CraneMapWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CraneMapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void CraneMapWidget::deviceSelected(FrameDevice::DeviceType _t1, QSharedPointer<SetOfOHBInfo> _t2, QSharedPointer<FoupOfOHBInfo> _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CraneMapWidget::configLoadStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CraneMapWidget::configLoadProgress(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CraneMapWidget::configLoadFinished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
