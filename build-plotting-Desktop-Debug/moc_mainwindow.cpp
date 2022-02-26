/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../GUI/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[22];
    char stringdata0[270];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 15), // "makePlotSensor1"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 15), // "makePlotSensor2"
QT_MOC_LITERAL(4, 44, 15), // "makePlotSensor3"
QT_MOC_LITERAL(5, 60, 16), // "makePlotFiltered"
QT_MOC_LITERAL(6, 77, 11), // "makePicMode"
QT_MOC_LITERAL(7, 89, 14), // "makePicBattery"
QT_MOC_LITERAL(8, 104, 11), // "makePicRoll"
QT_MOC_LITERAL(9, 116, 12), // "makePicPitch"
QT_MOC_LITERAL(10, 129, 10), // "makePicYaw"
QT_MOC_LITERAL(11, 140, 11), // "makePicLift"
QT_MOC_LITERAL(12, 152, 10), // "makePicAE0"
QT_MOC_LITERAL(13, 163, 10), // "makePicAE1"
QT_MOC_LITERAL(14, 174, 10), // "makePicAE2"
QT_MOC_LITERAL(15, 185, 10), // "makePicAE3"
QT_MOC_LITERAL(16, 196, 13), // "updateAllData"
QT_MOC_LITERAL(17, 210, 12), // "updateJSData"
QT_MOC_LITERAL(18, 223, 9), // "stoptimer"
QT_MOC_LITERAL(19, 233, 13), // "continuetimer"
QT_MOC_LITERAL(20, 247, 11), // "StopBTNTest"
QT_MOC_LITERAL(21, 259, 10) // "ConBTNTest"

    },
    "MainWindow\0makePlotSensor1\0\0makePlotSensor2\0"
    "makePlotSensor3\0makePlotFiltered\0"
    "makePicMode\0makePicBattery\0makePicRoll\0"
    "makePicPitch\0makePicYaw\0makePicLift\0"
    "makePicAE0\0makePicAE1\0makePicAE2\0"
    "makePicAE3\0updateAllData\0updateJSData\0"
    "stoptimer\0continuetimer\0StopBTNTest\0"
    "ConBTNTest"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x08 /* Private */,
       3,    0,  115,    2, 0x08 /* Private */,
       4,    0,  116,    2, 0x08 /* Private */,
       5,    0,  117,    2, 0x08 /* Private */,
       6,    0,  118,    2, 0x08 /* Private */,
       7,    0,  119,    2, 0x08 /* Private */,
       8,    0,  120,    2, 0x08 /* Private */,
       9,    0,  121,    2, 0x08 /* Private */,
      10,    0,  122,    2, 0x08 /* Private */,
      11,    0,  123,    2, 0x08 /* Private */,
      12,    0,  124,    2, 0x08 /* Private */,
      13,    0,  125,    2, 0x08 /* Private */,
      14,    0,  126,    2, 0x08 /* Private */,
      15,    0,  127,    2, 0x08 /* Private */,
      16,    0,  128,    2, 0x08 /* Private */,
      17,    0,  129,    2, 0x08 /* Private */,
      18,    0,  130,    2, 0x08 /* Private */,
      19,    0,  131,    2, 0x08 /* Private */,
      20,    0,  132,    2, 0x08 /* Private */,
      21,    0,  133,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->makePlotSensor1(); break;
        case 1: _t->makePlotSensor2(); break;
        case 2: _t->makePlotSensor3(); break;
        case 3: _t->makePlotFiltered(); break;
        case 4: _t->makePicMode(); break;
        case 5: _t->makePicBattery(); break;
        case 6: _t->makePicRoll(); break;
        case 7: _t->makePicPitch(); break;
        case 8: _t->makePicYaw(); break;
        case 9: _t->makePicLift(); break;
        case 10: _t->makePicAE0(); break;
        case 11: _t->makePicAE1(); break;
        case 12: _t->makePicAE2(); break;
        case 13: _t->makePicAE3(); break;
        case 14: _t->updateAllData(); break;
        case 15: _t->updateJSData(); break;
        case 16: _t->stoptimer(); break;
        case 17: _t->continuetimer(); break;
        case 18: _t->StopBTNTest(); break;
        case 19: _t->ConBTNTest(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
