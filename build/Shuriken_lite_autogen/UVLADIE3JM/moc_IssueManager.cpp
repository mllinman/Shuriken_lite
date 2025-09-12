/****************************************************************************
** Meta object code from reading C++ file 'IssueManager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/IssueManager.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IssueManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN12IssueManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto IssueManager::qt_create_metaobjectdata<qt_meta_tag_ZN12IssueManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "IssueManager",
        "issuesListed",
        "",
        "issues",
        "issueCreated",
        "title",
        "issueUpdated",
        "issueId",
        "status",
        "errorOccurred",
        "error",
        "commentsListed",
        "comments",
        "commentAdded",
        "body"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'issuesListed'
        QtMocHelpers::SignalData<void(QStringList)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 3 },
        }}),
        // Signal 'issueCreated'
        QtMocHelpers::SignalData<void(QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'issueUpdated'
        QtMocHelpers::SignalData<void(int, QString)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(QString)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'commentsListed'
        QtMocHelpers::SignalData<void(QStringList)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 12 },
        }}),
        // Signal 'commentAdded'
        QtMocHelpers::SignalData<void(QString)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<IssueManager, qt_meta_tag_ZN12IssueManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject IssueManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12IssueManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12IssueManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12IssueManagerE_t>.metaTypes,
    nullptr
} };

void IssueManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<IssueManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->issuesListed((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 1: _t->issueCreated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->issueUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->commentsListed((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 5: _t->commentAdded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (IssueManager::*)(QStringList )>(_a, &IssueManager::issuesListed, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (IssueManager::*)(QString )>(_a, &IssueManager::issueCreated, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (IssueManager::*)(int , QString )>(_a, &IssueManager::issueUpdated, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (IssueManager::*)(QString )>(_a, &IssueManager::errorOccurred, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (IssueManager::*)(QStringList )>(_a, &IssueManager::commentsListed, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (IssueManager::*)(QString )>(_a, &IssueManager::commentAdded, 5))
            return;
    }
}

const QMetaObject *IssueManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IssueManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12IssueManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IssueManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void IssueManager::issuesListed(QStringList _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void IssueManager::issueCreated(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void IssueManager::issueUpdated(int _t1, QString _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void IssueManager::errorOccurred(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void IssueManager::commentsListed(QStringList _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void IssueManager::commentAdded(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
