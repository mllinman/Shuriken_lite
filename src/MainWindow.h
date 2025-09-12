
#pragma once
#include <QMainWindow>
#include <QProcess>
#include <QFileSystemModel>
#include "CodeEditor.h"
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QHostAddress>
#include <QTreeWidget>
#include <QTreeView>

class Debugger;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
        void browseSource();
        void buildApp();
        void packageInstaller();
        void onFileDoubleClicked(const QModelIndex &index);
        void newFile();
        void deleteFile();
        void renameFile();
        void closeTab(int index);
        void gitStatus();
        void gitCommit();
        void gitPush();
        void gitPull();      
    void startCollaboration();

private:
    Ui::MainWindow *ui;
        QString builtExe;
        QProcess *builderProcess = nullptr;
        void parseAndDisplayLog(const QString &log);
        void onLogLinkClicked(const QUrl &url);
        void loadFile(const QString &filePath);

        QTabWidget *editorTabs;
        QMap<QString, CodeEditor*> openEditors; // path → editor
        QPlainTextEdit *gitLog;
        QPushButton *gitCommitBtn;
        QPushButton *gitPushBtn;
        QPushButton *gitPullBtn;

        QFileSystemModel *fsModel;
        QTreeWidget *watchTree;
        Debugger *debugger;
        QTreeView *fileTree;
        CodeEditor *editor;
        CodeEditor *currentEditor = nullptr;

        QString projectRoot;
        void newProject();
        void openProject();
        void cancelBuild();
    QWebSocketServer *server;
    QList<QWebSocket*> clients;
};