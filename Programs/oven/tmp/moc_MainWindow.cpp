/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Tue Oct 26 16:17:55 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/system/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LoadThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      23,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      42,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LoadThread[] = {
    "LoadThread\0\0finished()\0finishedError(int)\0"
    "get()\0"
};

const QMetaObject LoadThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_LoadThread,
      qt_meta_data_LoadThread, 0 }
};

const QMetaObject *LoadThread::metaObject() const
{
    return &staticMetaObject;
}

void *LoadThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoadThread))
        return static_cast<void*>(const_cast< LoadThread*>(this));
    return QThread::qt_metacast(_clname);
}

int LoadThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        case 1: finishedError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: get(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void LoadThread::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void LoadThread::finishedError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_SaveThread[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_SaveThread[] = {
    "SaveThread\0\0finished()\0"
};

const QMetaObject SaveThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SaveThread,
      qt_meta_data_SaveThread, 0 }
};

const QMetaObject *SaveThread::metaObject() const
{
    return &staticMetaObject;
}

void *SaveThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SaveThread))
        return static_cast<void*>(const_cast< SaveThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SaveThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finished(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SaveThread::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      32,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      34,   11,   11,   11, 0x0a,
      57,   11,   11,   11, 0x0a,
      86,   11,   11,   11, 0x0a,
     116,  114,   11,   11, 0x0a,
     173,   11,   11,   11, 0x0a,
     184,   11,   11,   11, 0x0a,
     197,   11,   11,   11, 0x0a,
     208,   11,   11,   11, 0x0a,
     231,   11,   11,   11, 0x0a,
     290,  278,   11,   11, 0x0a,
     328,   11,   11,   11, 0x0a,
     342,   11,   11,   11, 0x0a,
     356,   11,   11,   11, 0x0a,
     380,  367,   11,   11, 0x0a,
     398,   11,   11,   11, 0x0a,
     413,   11,   11,   11, 0x0a,
     429,   11,   11,   11, 0x0a,
     445,   11,   11,   11, 0x0a,
     470,  462,   11,   11, 0x0a,
     499,   11,   11,   11, 0x0a,
     513,   11,   11,   11, 0x0a,
     552,   11,   11,   11, 0x0a,
     594,   11,   11,   11, 0x0a,
     629,   11,   11,   11, 0x0a,
     663,   11,   11,   11, 0x0a,
     703,   11,   11,   11, 0x0a,
     745,   11,   11,   11, 0x0a,
     787,   11,   11,   11, 0x0a,
     813,   11,   11,   11, 0x0a,
     836,   11,   11,   11, 0x0a,
     863,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0updateQSliderFrames()\0"
    "updateQSpinBoxFrames()\0"
    "updateObjectInspector(ovInt)\0"
    "updatePropertyEditor(ovInt)\0,\0"
    "setSelectedProperties(QTreeWidgetItem*,QTreeWidgetItem*)\0"
    "loadFile()\0saveFrames()\0setScene()\0"
    "printSceneProperties()\0"
    "printAnimationFrameProperties(AnimationFrame&)\0"
    "frameNumber\0printAnimationFrameProperties(ovUInt)\0"
    "getError(int)\0startBaking()\0getFrame()\0"
    "milliseconds\0setAnimation(int)\0"
    "setNextFrame()\0playAnimation()\0"
    "showLastFrame()\0showFirstFrame()\0"
    "message\0displayErrorMessage(QString)\0"
    "setLiveSave()\0setParticleRenderingTypePosition(bool)\0"
    "setParticleRenderingTypeTemperature(bool)\0"
    "setParticleRenderingTypeSize(bool)\0"
    "setParticleRenderingTypeCO2(bool)\0"
    "setParticleRenderingTypeOnSurface(bool)\0"
    "setParticleRenderingTypeInCollision(bool)\0"
    "setParticleRenderingTypeTrueSurface(bool)\0"
    "updateSplatRadius(double)\0"
    "updateSplatRadius(int)\0"
    "updateCameraValueWidgets()\0"
    "updateCameraValues(double)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateQSliderFrames(); break;
        case 1: updateQSpinBoxFrames(); break;
        case 2: updateObjectInspector((*reinterpret_cast< ovInt(*)>(_a[1]))); break;
        case 3: updatePropertyEditor((*reinterpret_cast< ovInt(*)>(_a[1]))); break;
        case 4: setSelectedProperties((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        case 5: loadFile(); break;
        case 6: saveFrames(); break;
        case 7: setScene(); break;
        case 8: printSceneProperties(); break;
        case 9: printAnimationFrameProperties((*reinterpret_cast< AnimationFrame(*)>(_a[1]))); break;
        case 10: printAnimationFrameProperties((*reinterpret_cast< ovUInt(*)>(_a[1]))); break;
        case 11: getError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: startBaking(); break;
        case 13: getFrame(); break;
        case 14: setAnimation((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: setNextFrame(); break;
        case 16: playAnimation(); break;
        case 17: showLastFrame(); break;
        case 18: showFirstFrame(); break;
        case 19: displayErrorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 20: setLiveSave(); break;
        case 21: setParticleRenderingTypePosition((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: setParticleRenderingTypeTemperature((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: setParticleRenderingTypeSize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: setParticleRenderingTypeCO2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: setParticleRenderingTypeOnSurface((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: setParticleRenderingTypeInCollision((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: setParticleRenderingTypeTrueSurface((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: updateSplatRadius((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 29: updateSplatRadius((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: updateCameraValueWidgets(); break;
        case 31: updateCameraValues((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 32;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
