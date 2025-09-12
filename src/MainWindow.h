
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
#include <QListWidgetItem>

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
    void onFileSelected(QListWidgetItem *item);
    void showFileContextMenu(const QPoint &pos);
    void setupTerminalPanel();
    void setupSearchPanel();
    void setupDashboard();
    void setupAIChat();
    void setupPropertyEditor();
    void setupCommitChart();
    void updateVariables();
    void updateStackTrace();
    void updateMemory(QString addr);
    void gitDiff();
    void loadFile(const QString &filePath);
    void parseAndDisplayLog(const QString &log);
    void onLogLinkClicked(const QUrl &url);
    void formatFile(QString file);
    void installPlugin(QString repoUrl);
    void exportCICD(QString target);
    void saveSecret(QString key, QString value);
    QString loadSecret(QString key);
    void pushArtifact(QString path, QString registry);
    void generateDebPackage(const QString &appName, const QString &version);
    void generateAppImage(const QString &appName, const QString &version);
    void generateDMG(const QString &appName, const QString &version);
    void runTests();
    void generateUML(QString projectRoot);
    void generateRoadmap();
    void updateWatches();
    void showPipelineLogs();
    void searchPackage(QString name);
    void installPackage(QString name);
    void renameSymbol(QString file, int line, int col, QString newName);
    void runInContainer(QString image, QString appPath);
    void runInVM(QString isoPath);
    void reviewCode(QString filePath);
    void commitChanges();

private:
    Ui::MainWindow *ui;
    QString builtExe;
    QProcess *builderProcess = nullptr;
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
    QWebSocketServer *server;
    QList<QWebSocket*> clients;
};