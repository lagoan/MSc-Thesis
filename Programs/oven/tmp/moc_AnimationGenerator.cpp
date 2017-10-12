/****************************************************************************
** Meta object code from reading C++ file 'AnimationGenerator.h'
**
** Created: Tue Oct 26 16:18:14 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/system/AnimationGenerator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnimationGenerator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AnimationGenerator[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x05,
      31,   19,   19,   19, 0x05,
      49,   19,   19,   19, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_AnimationGenerator[] = {
    "AnimationGenerator\0\0newFrame()\0"
    "newAnimation(int)\0errorMessage(QString)\0"
};

const QMetaObject AnimationGenerator::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_AnimationGenerator,
      qt_meta_data_AnimationGenerator, 0 }
};

const QMetaObject *AnimationGenerator::metaObject() const
{
    return &staticMetaObject;
}

void *AnimationGenerator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AnimationGenerator))
        return static_cast<void*>(const_cast< AnimationGenerator*>(this));
    return QThread::qt_metacast(_clname);
}

int AnimationGenerator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newFrame(); break;
        case 1: newAnimation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: errorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void AnimationGenerator::newFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AnimationGenerator::newAnimation(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AnimationGenerator::errorMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
