/****************************************************************************
** Meta object code from reading C++ file 'SimulationProperties.h'
**
** Created: Tue Oct 26 16:18:02 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/system/SimulationProperties.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SimulationProperties.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimulationProperties[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SimulationProperties[] = {
    "SimulationProperties\0\0updatedValues()\0"
};

const QMetaObject SimulationProperties::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SimulationProperties,
      qt_meta_data_SimulationProperties, 0 }
};

const QMetaObject *SimulationProperties::metaObject() const
{
    return &staticMetaObject;
}

void *SimulationProperties::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimulationProperties))
        return static_cast<void*>(const_cast< SimulationProperties*>(this));
    return QObject::qt_metacast(_clname);
}

int SimulationProperties::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updatedValues(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SimulationProperties::updatedValues()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
