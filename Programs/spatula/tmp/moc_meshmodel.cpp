/****************************************************************************
** Meta object code from reading C++ file 'meshmodel.h'
**
** Created: Sun Jan 30 19:54:44 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/meshlab/meshmodel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meshmodel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MeshDocument[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_MeshDocument[] = {
    "MeshDocument\0\0index\0currentMeshChanged(int)\0"
};

const QMetaObject MeshDocument::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MeshDocument,
      qt_meta_data_MeshDocument, 0 }
};

const QMetaObject *MeshDocument::metaObject() const
{
    return &staticMetaObject;
}

void *MeshDocument::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MeshDocument))
        return static_cast<void*>(const_cast< MeshDocument*>(this));
    return QObject::qt_metacast(_clname);
}

int MeshDocument::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentMeshChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void MeshDocument::currentMeshChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
