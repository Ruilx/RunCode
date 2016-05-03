/****************************************************************************
** Meta object code from reading C++ file 'runprocess.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "runprocess.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runprocess.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RunProcess_t {
    QByteArrayData data[26];
    char stringdata0[396];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunProcess_t qt_meta_stringdata_RunProcess = {
    {
QT_MOC_LITERAL(0, 0, 10), // "RunProcess"
QT_MOC_LITERAL(1, 11, 10), // "stoppedSig"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 10), // "runningSig"
QT_MOC_LITERAL(4, 34, 15), // "requestMMtoStop"
QT_MOC_LITERAL(5, 50, 16), // "analysisFinished"
QT_MOC_LITERAL(6, 67, 13), // "sendStdOutput"
QT_MOC_LITERAL(7, 81, 12), // "sendStdError"
QT_MOC_LITERAL(8, 94, 13), // "errorOccurred"
QT_MOC_LITERAL(9, 108, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(10, 131, 5), // "error"
QT_MOC_LITERAL(11, 137, 8), // "finished"
QT_MOC_LITERAL(12, 146, 8), // "exitCode"
QT_MOC_LITERAL(13, 155, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(14, 176, 10), // "exitStatus"
QT_MOC_LITERAL(15, 187, 17), // "readStandardError"
QT_MOC_LITERAL(16, 205, 18), // "readStandardOutput"
QT_MOC_LITERAL(17, 224, 12), // "stateChanged"
QT_MOC_LITERAL(18, 237, 22), // "QProcess::ProcessState"
QT_MOC_LITERAL(19, 260, 8), // "newState"
QT_MOC_LITERAL(20, 269, 26), // "stopProgramBecauseOfMemory"
QT_MOC_LITERAL(21, 296, 24), // "stopProgramBecauseOfTime"
QT_MOC_LITERAL(22, 321, 23), // "stopProgramSuccessfully"
QT_MOC_LITERAL(23, 345, 25), // "stopProgramBecauseCrashed"
QT_MOC_LITERAL(24, 371, 12), // "startProgram"
QT_MOC_LITERAL(25, 384, 11) // "stopProgram"

    },
    "RunProcess\0stoppedSig\0\0runningSig\0"
    "requestMMtoStop\0analysisFinished\0"
    "sendStdOutput\0sendStdError\0errorOccurred\0"
    "QProcess::ProcessError\0error\0finished\0"
    "exitCode\0QProcess::ExitStatus\0exitStatus\0"
    "readStandardError\0readStandardOutput\0"
    "stateChanged\0QProcess::ProcessState\0"
    "newState\0stopProgramBecauseOfMemory\0"
    "stopProgramBecauseOfTime\0"
    "stopProgramSuccessfully\0"
    "stopProgramBecauseCrashed\0startProgram\0"
    "stopProgram"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunProcess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    0,  101,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,
       6,    1,  103,    2, 0x06 /* Public */,
       7,    1,  106,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,  109,    2, 0x08 /* Private */,
      11,    2,  112,    2, 0x08 /* Private */,
      15,    0,  117,    2, 0x08 /* Private */,
      16,    0,  118,    2, 0x08 /* Private */,
      17,    1,  119,    2, 0x08 /* Private */,
      20,    0,  122,    2, 0x08 /* Private */,
      21,    0,  123,    2, 0x08 /* Private */,
      22,    0,  124,    2, 0x08 /* Private */,
      23,    0,  125,    2, 0x08 /* Private */,
      24,    0,  126,    2, 0x0a /* Public */,
      25,    0,  127,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 13,   12,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RunProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunProcess *_t = static_cast<RunProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stoppedSig(); break;
        case 1: _t->runningSig(); break;
        case 2: _t->requestMMtoStop(); break;
        case 3: _t->analysisFinished(); break;
        case 4: _t->sendStdOutput((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->sendStdError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->errorOccurred((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 7: _t->finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 8: _t->readStandardError(); break;
        case 9: _t->readStandardOutput(); break;
        case 10: _t->stateChanged((*reinterpret_cast< QProcess::ProcessState(*)>(_a[1]))); break;
        case 11: _t->stopProgramBecauseOfMemory(); break;
        case 12: _t->stopProgramBecauseOfTime(); break;
        case 13: _t->stopProgramSuccessfully(); break;
        case 14: _t->stopProgramBecauseCrashed(); break;
        case 15: _t->startProgram(); break;
        case 16: _t->stopProgram(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RunProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunProcess::stoppedSig)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RunProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunProcess::runningSig)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RunProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunProcess::requestMMtoStop)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RunProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunProcess::analysisFinished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RunProcess::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunProcess::sendStdOutput)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (RunProcess::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RunProcess::sendStdError)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject RunProcess::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RunProcess.data,
      qt_meta_data_RunProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RunProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RunProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RunProcess.stringdata0))
        return static_cast<void*>(const_cast< RunProcess*>(this));
    return QObject::qt_metacast(_clname);
}

int RunProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void RunProcess::stoppedSig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void RunProcess::runningSig()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void RunProcess::requestMMtoStop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void RunProcess::analysisFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void RunProcess::sendStdOutput(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RunProcess::sendStdError(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
