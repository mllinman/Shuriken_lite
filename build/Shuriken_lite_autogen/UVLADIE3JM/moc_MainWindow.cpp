/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/MainWindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "browseSource",
        "",
        "buildApp",
        "packageInstaller",
        "onFileDoubleClicked",
        "QModelIndex",
        "index",
        "newFile",
        "deleteFile",
        "renameFile",
        "closeTab",
        "gitStatus",
        "gitCommit",
        "gitPush",
        "gitPull",
        "startCollaboration",
        "onFileSelected",
        "QListWidgetItem*",
        "item",
        "showFileContextMenu",
        "pos",
        "setupTerminalPanel",
        "setupSearchPanel",
        "setupDashboard",
        "setupAIChat",
        "setupPropertyEditor",
        "setupCommitChart",
        "updateVariables",
        "updateStackTrace",
        "updateMemory",
        "addr",
        "gitDiff",
        "loadFile",
        "filePath",
        "parseAndDisplayLog",
        "log",
        "onLogLinkClicked",
        "url",
        "formatFile",
        "file",
        "installPlugin",
        "repoUrl",
        "exportCICD",
        "target",
        "saveSecret",
        "key",
        "value",
        "loadSecret",
        "pushArtifact",
        "path",
        "registry",
        "generateDebPackage",
        "appName",
        "version",
        "generateAppImage",
        "generateDMG",
        "runTests",
        "generateUML",
        "projectRoot",
        "generateRoadmap",
        "updateWatches",
        "showPipelineLogs",
        "searchPackage",
        "name",
        "installPackage",
        "renameSymbol",
        "line",
        "col",
        "newName",
        "runInContainer",
        "image",
        "appPath",
        "runInVM",
        "isoPath",
        "reviewCode",
        "commitChanges"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'browseSource'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'buildApp'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'packageInstaller'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFileDoubleClicked'
        QtMocHelpers::SlotData<void(const QModelIndex &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'newFile'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'deleteFile'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'renameFile'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'closeTab'
        QtMocHelpers::SlotData<void(int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 7 },
        }}),
        // Slot 'gitStatus'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'gitCommit'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'gitPush'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'gitPull'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'startCollaboration'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFileSelected'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'showFileContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QPoint, 21 },
        }}),
        // Slot 'setupTerminalPanel'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setupSearchPanel'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setupDashboard'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setupAIChat'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setupPropertyEditor'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setupCommitChart'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateVariables'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateStackTrace'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateMemory'
        QtMocHelpers::SlotData<void(QString)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 31 },
        }}),
        // Slot 'gitDiff'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadFile'
        QtMocHelpers::SlotData<void(const QString &)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Slot 'parseAndDisplayLog'
        QtMocHelpers::SlotData<void(const QString &)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 36 },
        }}),
        // Slot 'onLogLinkClicked'
        QtMocHelpers::SlotData<void(const QUrl &)>(37, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QUrl, 38 },
        }}),
        // Slot 'formatFile'
        QtMocHelpers::SlotData<void(QString)>(39, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 40 },
        }}),
        // Slot 'installPlugin'
        QtMocHelpers::SlotData<void(QString)>(41, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 42 },
        }}),
        // Slot 'exportCICD'
        QtMocHelpers::SlotData<void(QString)>(43, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 44 },
        }}),
        // Slot 'saveSecret'
        QtMocHelpers::SlotData<void(QString, QString)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 46 }, { QMetaType::QString, 47 },
        }}),
        // Slot 'loadSecret'
        QtMocHelpers::SlotData<QString(QString)>(48, 2, QMC::AccessPrivate, QMetaType::QString, {{
            { QMetaType::QString, 46 },
        }}),
        // Slot 'pushArtifact'
        QtMocHelpers::SlotData<void(QString, QString)>(49, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 50 }, { QMetaType::QString, 51 },
        }}),
        // Slot 'generateDebPackage'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(52, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 53 }, { QMetaType::QString, 54 },
        }}),
        // Slot 'generateAppImage'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(55, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 53 }, { QMetaType::QString, 54 },
        }}),
        // Slot 'generateDMG'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(56, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 53 }, { QMetaType::QString, 54 },
        }}),
        // Slot 'runTests'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'generateUML'
        QtMocHelpers::SlotData<void(QString)>(58, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 59 },
        }}),
        // Slot 'generateRoadmap'
        QtMocHelpers::SlotData<void()>(60, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateWatches'
        QtMocHelpers::SlotData<void()>(61, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'showPipelineLogs'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'searchPackage'
        QtMocHelpers::SlotData<void(QString)>(63, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 64 },
        }}),
        // Slot 'installPackage'
        QtMocHelpers::SlotData<void(QString)>(65, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 64 },
        }}),
        // Slot 'renameSymbol'
        QtMocHelpers::SlotData<void(QString, int, int, QString)>(66, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 40 }, { QMetaType::Int, 67 }, { QMetaType::Int, 68 }, { QMetaType::QString, 69 },
        }}),
        // Slot 'runInContainer'
        QtMocHelpers::SlotData<void(QString, QString)>(70, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 71 }, { QMetaType::QString, 72 },
        }}),
        // Slot 'runInVM'
        QtMocHelpers::SlotData<void(QString)>(73, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 74 },
        }}),
        // Slot 'reviewCode'
        QtMocHelpers::SlotData<void(QString)>(75, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Slot 'commitChanges'
        QtMocHelpers::SlotData<void()>(76, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->browseSource(); break;
        case 1: _t->buildApp(); break;
        case 2: _t->packageInstaller(); break;
        case 3: _t->onFileDoubleClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 4: _t->newFile(); break;
        case 5: _t->deleteFile(); break;
        case 6: _t->renameFile(); break;
        case 7: _t->closeTab((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->gitStatus(); break;
        case 9: _t->gitCommit(); break;
        case 10: _t->gitPush(); break;
        case 11: _t->gitPull(); break;
        case 12: _t->startCollaboration(); break;
        case 13: _t->onFileSelected((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 14: _t->showFileContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 15: _t->setupTerminalPanel(); break;
        case 16: _t->setupSearchPanel(); break;
        case 17: _t->setupDashboard(); break;
        case 18: _t->setupAIChat(); break;
        case 19: _t->setupPropertyEditor(); break;
        case 20: _t->setupCommitChart(); break;
        case 21: _t->updateVariables(); break;
        case 22: _t->updateStackTrace(); break;
        case 23: _t->updateMemory((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 24: _t->gitDiff(); break;
        case 25: _t->loadFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 26: _t->parseAndDisplayLog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 27: _t->onLogLinkClicked((*reinterpret_cast< std::add_pointer_t<QUrl>>(_a[1]))); break;
        case 28: _t->formatFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 29: _t->installPlugin((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 30: _t->exportCICD((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 31: _t->saveSecret((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 32: { QString _r = _t->loadSecret((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 33: _t->pushArtifact((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 34: _t->generateDebPackage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 35: _t->generateAppImage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 36: _t->generateDMG((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 37: _t->runTests(); break;
        case 38: _t->generateUML((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 39: _t->generateRoadmap(); break;
        case 40: _t->updateWatches(); break;
        case 41: _t->showPipelineLogs(); break;
        case 42: _t->searchPackage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 43: _t->installPackage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 44: _t->renameSymbol((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 45: _t->runInContainer((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 46: _t->runInVM((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 47: _t->reviewCode((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 48: _t->commitChanges(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 49)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 49;
    }
    return _id;
}
QT_WARNING_POP
