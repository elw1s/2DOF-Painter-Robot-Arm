/****************************************************************************
** Meta object code from reading C++ file 'DrawingApp.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../scara_gui/DrawingApp.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DrawingApp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSDrawingAppENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSDrawingAppENDCLASS = QtMocHelpers::stringData(
    "DrawingApp",
    "saveImage",
    "",
    "setPenStroke",
    "stroke",
    "setEraserTrue",
    "setEraserFalse",
    "createStyledButton",
    "QPushButton*",
    "icon",
    "size",
    "textColor",
    "borderColor",
    "QWidget*",
    "parent"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSDrawingAppENDCLASS_t {
    uint offsetsAndSizes[30];
    char stringdata0[11];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[7];
    char stringdata5[14];
    char stringdata6[15];
    char stringdata7[19];
    char stringdata8[13];
    char stringdata9[5];
    char stringdata10[5];
    char stringdata11[10];
    char stringdata12[12];
    char stringdata13[9];
    char stringdata14[7];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSDrawingAppENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSDrawingAppENDCLASS_t qt_meta_stringdata_CLASSDrawingAppENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "DrawingApp"
        QT_MOC_LITERAL(11, 9),  // "saveImage"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 12),  // "setPenStroke"
        QT_MOC_LITERAL(35, 6),  // "stroke"
        QT_MOC_LITERAL(42, 13),  // "setEraserTrue"
        QT_MOC_LITERAL(56, 14),  // "setEraserFalse"
        QT_MOC_LITERAL(71, 18),  // "createStyledButton"
        QT_MOC_LITERAL(90, 12),  // "QPushButton*"
        QT_MOC_LITERAL(103, 4),  // "icon"
        QT_MOC_LITERAL(108, 4),  // "size"
        QT_MOC_LITERAL(113, 9),  // "textColor"
        QT_MOC_LITERAL(123, 11),  // "borderColor"
        QT_MOC_LITERAL(135, 8),  // "QWidget*"
        QT_MOC_LITERAL(144, 6)   // "parent"
    },
    "DrawingApp",
    "saveImage",
    "",
    "setPenStroke",
    "stroke",
    "setEraserTrue",
    "setEraserFalse",
    "createStyledButton",
    "QPushButton*",
    "icon",
    "size",
    "textColor",
    "borderColor",
    "QWidget*",
    "parent"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSDrawingAppENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x08,    1 /* Private */,
       3,    1,   45,    2, 0x08,    2 /* Private */,
       5,    0,   48,    2, 0x08,    4 /* Private */,
       6,    0,   49,    2, 0x08,    5 /* Private */,
       7,    5,   50,    2, 0x08,    6 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 8, QMetaType::QIcon, QMetaType::QSize, QMetaType::QString, QMetaType::QString, 0x80000000 | 13,    9,   10,   11,   12,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject DrawingApp::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSDrawingAppENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSDrawingAppENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSDrawingAppENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<DrawingApp, std::true_type>,
        // method 'saveImage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setPenStroke'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'setEraserTrue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setEraserFalse'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'createStyledButton'
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QIcon &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSize &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QWidget *, std::false_type>
    >,
    nullptr
} };

void DrawingApp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DrawingApp *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->saveImage(); break;
        case 1: _t->setPenStroke((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->setEraserTrue(); break;
        case 3: _t->setEraserFalse(); break;
        case 4: { QPushButton* _r = _t->createStyledButton((*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSize>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<QWidget*>>(_a[5])));
            if (_a[0]) *reinterpret_cast< QPushButton**>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 4:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *DrawingApp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DrawingApp::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSDrawingAppENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DrawingApp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
