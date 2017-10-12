/****************************************************************************
** Meta object code from reading C++ file 'ViewPort.h'
**
** Created: Tue Oct 26 16:17:47 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/system/ViewPort.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ViewPort.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ViewPort[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      36,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,    9,    9,    9, 0x0a,
      71,   69,    9,    9, 0x0a,
     129,  124,    9,    9, 0x0a,
     152,  124,    9,    9, 0x0a,
     173,  124,    9,    9, 0x0a,
     194,  124,    9,    9, 0x0a,
     223,  124,    9,    9, 0x0a,
     245,  124,    9,    9, 0x0a,
     272,  267,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ViewPort[] = {
    "ViewPort\0\0selectOnObjectList(ovInt)\0"
    "cameraChanged()\0updateViewPort()\0,\0"
    "setSelectedObject(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "show\0setViewParticles(bool)\0"
    "setViewSurface(bool)\0setViewCutOuts(bool)\0"
    "setViewBubbleParticles(bool)\0"
    "setViewGradient(bool)\0setViewVelocity(bool)\0"
    "type\0setParticleRenderingType(ParticleRenderingType)\0"
};

const QMetaObject ViewPort::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_ViewPort,
      qt_meta_data_ViewPort, 0 }
};

const QMetaObject *ViewPort::metaObject() const
{
    return &staticMetaObject;
}

void *ViewPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ViewPort))
        return static_cast<void*>(const_cast< ViewPort*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int ViewPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: selectOnObjectList((*reinterpret_cast< ovInt(*)>(_a[1]))); break;
        case 1: cameraChanged(); break;
        case 2: updateViewPort(); break;
        case 3: setSelectedObject((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 4: setViewParticles((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: setViewSurface((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: setViewCutOuts((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: setViewBubbleParticles((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: setViewGradient((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: setViewVelocity((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: setParticleRenderingType((*reinterpret_cast< ParticleRenderingType(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ViewPort::selectOnObjectList(ovInt _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ViewPort::cameraChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
