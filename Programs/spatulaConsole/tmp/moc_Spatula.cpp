/****************************************************************************
** Meta object code from reading C++ file 'Spatula.h'
**
** Created: Sun Nov 14 10:34:50 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/Spatula.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Spatula.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Spatula2[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      32,    9,    9,    9, 0x0a,
      58,    9,    9,    9, 0x0a,
      78,    9,    9,    9, 0x0a,
      99,    9,    9,    9, 0x0a,
     122,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Spatula2[] = {
    "Spatula2\0\0setWorkingDirectory()\0"
    "processWorkingDirectory()\0updateProgress(int)\0"
    "setFileProgress(int)\0setActionProgress(int)\0"
    "updateLog(QString)\0"
};

const QMetaObject Spatula2::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Spatula2,
      qt_meta_data_Spatula2, 0 }
};

const QMetaObject *Spatula2::metaObject() const
{
    return &staticMetaObject;
}

void *Spatula2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Spatula2))
        return static_cast<void*>(const_cast< Spatula2*>(this));
    return QObject::qt_metacast(_clname);
}

int Spatula2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setWorkingDirectory(); break;
        case 1: processWorkingDirectory(); break;
        case 2: updateProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: setFileProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setActionProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: updateLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
