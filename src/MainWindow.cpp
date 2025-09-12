#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTextStream>
#include <QRegularExpression>
#include <QDockWidget>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QInputDialog>
#include <QMenu>
#include <QAction>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTabWidget>
#include <QListWidget>
#include <QCryptographicHash>
#include <QSettings>
#include <QWebSocket>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include "Builder.h"
#include "Installer.h"
#include "CodeEditor.h"
#include "Debugger.h"
#include "TerminalWidget.h"
#include "Updater.h"
#include "CodeViewer.h"
#include "Completer.h"
#include <clang-c/Index.h>
#include <QDirIterator>
#include <QFont>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QSplitter>
#include <QCryptographicHash>
#include <QSettings>
#include <QWebSocket>
#include <QNetworkAccessManager>
#include <QRegExp>
#include <QDebug>

QWebSocketServer *server = new QWebSocketServer("ShurikenCollab", QWebSocketServer::NonSecureMode);
QList<QWebSocket*> clients;

void MainWindow::startCollaboration() {
    if (!server->listen(QHostAddress::Any, 12345)) return;

    connect(server, &QWebSocketServer::newConnection, this, [=]() {
        QWebSocket *sock = server->nextPendingConnection();
        clients << sock;

        connect(sock, &QWebSocket::textMessageReceived, this, [=](QString msg) {
            for (QWebSocket *c : clients) c->sendTextMessage(msg);
        });
    });
}

void CodeEditor::broadcastChange(QString change) {
    for (QWebSocket *c : clients) {
        c->sendTextMessage(change);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseSource);
    connect(ui->buildButton, &QPushButton::clicked, this, &MainWindow::buildApp);
    connect(ui->packageButton, &QPushButton::clicked, this, &MainWindow::packageInstaller);
    connect(ui->fileListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onFileDoubleClicked);
    connect(ui->fileListWidget, &QListWidget::itemClicked, this, &MainWindow::onFileSelected);
    connect(ui->fileListWidget, &QListWidget::customContextMenuRequested, this, &MainWindow::showFileContextMenu);

    QDockWidget *dockWatch = new QDockWidget("Variables", this);
    watchTree = new QTreeWidget;
    watchTree->setHeaderLabels({"Name", "Value"});
    dockWatch->setWidget(watchTree);
    addDockWidget(Qt::RightDockWidgetArea, dockWatch);

    // Tabbed editor
    editorTabs = new QTabWidget(this);
    editorTabs->setTabsClosable(true);
    setCentralWidget(editorTabs);

    connect(editorTabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    ...    connect(ui->fileListWidget, &QListWidget::itemClicked, this, &MainWindow::onFileSelected);
    connect(ui->newProjectButton, &QPushButton::clicked, this, &MainWindow::newProject);
    connect(ui->openProjectButton, &QPushButton::clicked, this, &MainWindow::openProject);
    connect(ui->cancelBuildButton, &QPushButton::clicked, this, &MainWindow::cancelBuild);
    // 🔹 Git Dock
    QWidget *gitPanel = new QWidget(this);
    QVBoxLayout *gitLayout = new QVBoxLayout;

    gitLog = new QPlainTextEdit;
    gitLog->setReadOnly(true);

    gitCommitBtn = new QPushButton("Commit");
    gitPushBtn = new QPushButton("Push");
    gitPullBtn = new QPushButton("Pull");

    gitLayout->addWidget(new QLabel("Git Panel"));
    gitLayout->addWidget(gitLog);
    gitLayout->addWidget(gitCommitBtn);
    gitLayout->addWidget(gitPushBtn);
    gitLayout->addWidget(gitPullBtn);

    gitPanel->setLayout(gitLayout);

    QDockWidget *dockGit = new QDockWidget("Git", this);
    dockGit->setWidget(gitPanel);
    addDockWidget(Qt::BottomDockWidgetArea, dockGit);

    connect(gitCommitBtn, &QPushButton::clicked, this, &MainWindow::gitCommit);
    connect(gitPushBtn, &QPushButton::clicked, this, &MainWindow::gitPush);
    connect(gitPullBtn, &QPushButton::clicked, this, &MainWindow::gitPull);

    gitStatus();
}

void MainWindow::gitStatus() {
    QProcess git;
    git.setWorkingDirectory(projectRoot);
    git.start("git", {"status"});
    git.waitForFinished();
    gitLog->appendPlainText(git.readAllStandardOutput());
}

void MainWindow::gitCommit() {
    QString msg = QInputDialog::getText(this, "Commit", "Commit message:");
    if (msg.isEmpty()) return;

    QProcess git;
    git.setWorkingDirectory(projectRoot);
    git.start("git", {"add", "."});
    git.waitForFinished();
    git.start("git", {"commit", "-m", msg});
    git.waitForFinished();

    gitLog->appendPlainText("Commit: " + msg);
    gitStatus();
}

void MainWindow::gitPush() {
    QProcess git;
    git.setWorkingDirectory(projectRoot);
    git.start("git", {"push"});
    git.waitForFinished();
    gitLog->appendPlainText(git.readAllStandardOutput());
}

void MainWindow::gitPull() {
    QProcess git;
    git.setWorkingDirectory(projectRoot);
    git.start("git", {"pull"});
    git.waitForFinished();
    gitLog->appendPlainText(git.readAllStandardOutput());
}

MainWindow::~MainWindow() {
    delete ui;
    qDeleteAll(openEditors);
    delete editorTabs;
}

void MainWindow::browseSource() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Source Folder");
    if (!dir.isEmpty()) {
        ui->repoInput->setText(dir);
        projectRoot = dir;
        ui->logOutput->append("<font color='green'>Source folder selected: " + dir + "</font>");
        // Load project files
        QStringList files = QDir(dir).entryList(QStringList() << "*.cpp" << "*.h", QDir::Files);
        ui->fileListWidget->clear();
        for (const QString &file : files) {
            ui->fileListWidget->addItem(file);
        }
        if (!files.isEmpty()) {
            ui->fileListWidget->setCurrentRow(0);
            loadFile(dir + "/" + files.first());
        }
    }
}

void MainWindow::parseAndDisplayLog(const QString &log) {
    QStringList lines = log.split("\n");

    for (const QString &line : lines) {
        QString formatted = line;

        if (line.contains("error")) {
            ui->logOutput->append("<a href=\"" + line + "\"><font color='red'>" + line + "</font></a>");
        } else if (line.contains("warning")) {
            ui->logOutput->append("<font color='orange'>" + line + "</font>");
        } else {
            ui->logOutput->append(line);
        }
    }

    // Allow clickable links
    ui->logOutput->setOpenExternalLinks(false);
    connect(ui->logOutput, &QTextBrowser::anchorClicked, this, &MainWindow::onLogLinkClicked);
}
void MainWindow::setupTerminalPanel() {
    TerminalWidget *terminal = new TerminalWidget(this);

    QDockWidget *dockTerminal = new QDockWidget("Terminal", this);
    dockTerminal->setWidget(terminal);
    addDockWidget(Qt::BottomDockWidgetArea, dockTerminal);
}

void MainWindow::onLogLinkClicked(const QUrl &url) {
    QString line = url.toString();
    QRegularExpression re(R"((.*\.cpp):(\d+))"); // match file.cpp:line
    QRegularExpressionMatch match = re.match(line);

    if (match.hasMatch()) {
        QString filePath = match.captured(1);
        int lineNumber = match.captured(2).toInt();

        CodeViewer *viewer = new CodeViewer;
        viewer->setWindowTitle(filePath);
        viewer->resize(800, 600);
        viewer->show();
        viewer->loadFile(filePath, lineNumber);
    }
}

void MainWindow::buildApp() {
    QString sourceDir = ui->repoInput->text();
    if (sourceDir.isEmpty()) {
       ui->logOutput->append("<font color='red'>Please select a source folder first.</font>");
        return;
    }

    ui->progressBar->setValue(0);
    ui->statusBar->showMessage("Building project...");

    QString log;
    Builder builder("llvm/prebuilt_clang/");
    QString exeOut = QDir(sourceDir).filePath("OutputApp.exe");

    if (builder.compileProject(sourceDir, exeOut, log)) {
        ui->logOutput->append("<font color='green'>Build successful: " + exeOut + "</font>");
        builtExe = exeOut;
        ui->progressBar->setValue(100);
    } else {
        ui->logOutput->append("<font color='red'>Build failed.</font>");
    }}

    // Colorize log output
    QStringList lines = log.split("\n");
    for (const QString &line : lines) {
        if (line.contains("error")) ui->logOutput->append("<font color='red'>" + line + "</font>");
        else if (line.contains("warning")) ui->logOutput->append("<font color='orange'>" + line + "</font>");
        else ui->logOutput->append(line);
    }

    ui->statusBar->showMessage("Ready");
    QMessageBox::information(this, "Build Complete", "Project build process complete.");
    ui->progressBar->setValue(100);


void MainWindow::packageInstaller() {
    if (builtExe.isEmpty()) {
        ui->installerLogOutput->append("<font color='red'>No built executable to package.</font>");
        return;
    }

    QString appName = ui->appNameInput->text().isEmpty() ? "MyApp" : ui->appNameInput->text();
    QString publisher = ui->publisherInput->text().isEmpty() ? "Shuriken Builder" : ui->publisherInput->text();
    QString version = ui->versionInput->text().isEmpty() ? "1.0.0" : ui->versionInput->text();

    QString outInstaller = QFileDialog::getSaveFileName(this, "Save Installer", appName + "_Setup.exe");
    if (outInstaller.isEmpty()) return;

    ui->statusBar->showMessage("Packaging installer...");

    QString log;
    Installer installer(appName, builtExe, "manifest_example.json");
    if (installer.createInstaller(outInstaller, log)) {
        ui->installerLogOutput->append("<font color='green'>Installer created: " + outInstaller + "</font>");
    } else {
        ui->installerLogOutput->append("<font color='red'>Installer packaging failed.</font>");
    }
    ui->installerLogOutput->append(log);
    ui->statusBar->showMessage("Ready");

}
    QMessageBox::information(this, "Packaging Complete", "Installer packaging process complete.");
    ui->progressBar->setValue(100);
    ui->statusBar->showMessage("Ready");

void MainWindow::cancelBuild() {
    if (builderProcess && builderProcess->state() == QProcess::Running) {
        builderProcess->kill();
        ui->logOutput->append("<font color='orange'>Build cancelled by user.</font>");
        ui->statusBar->showMessage("Build cancelled");
        ui->progressBar->setValue(0);
    }
}
void MainWindow::updateVariables() {
    debugger->sendCommand("info locals");
    debugger->sendCommand("info args");

    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        watchTree->clear();
        QStringList lines = out.split("\n");
        for (const QString &line : lines) {
            if (line.contains("=")) {
                QStringList parts = line.split("=");
                QTreeWidgetItem *item = new QTreeWidgetItem(watchTree);
                item->setText(0, parts[0].trimmed());
                item->setText(1, parts[1].trimmed());
                watchTree->addTopLevelItem(item);
            }
        }
    });
void MainWindow::newProject() {
    QString type = QInputDialog::getItem(this, "New Project", "Select template:",
                                         {"Console App", "Qt GUI App", "Library"}, 0, false);

    QString dir = QFileDialog::getExistingDirectory(this, "Select Project Folder");
    if (dir.isEmpty()) return;

    if (type == "Console App") {
        QFile::copy("templates/console_app/main.cpp", dir + "/main.cpp");
    } else if (type == "Qt GUI App") {
        QFile::copy("templates/qt_gui_app/main.cpp", dir + "/main.cpp");
    } else if (type == "Library") {
        QFile::copy("templates/library/mylib.cpp", dir + "/mylib.cpp");
        QFile::copy("templates/library/mylib.h", dir + "/mylib.h");
    }

    ui->logOutput->append("<font color='green'>New project created: " + type + "</font>");
}
    ui->repoInput->setText(dir);
    projectRoot = dir;
    ui->fileListWidget->clear();
    QStringList files = QDir(dir).entryList(QStringList() << "*.cpp" << "*.h", QDir::Files);
    for (const QString &file : files) {
        ui->fileListWidget->addItem(file);
    }
    if (!files.isEmpty()) {
        ui->fileListWidget->setCurrentRow(0);
        loadFile(dir + "/" + files.first());
    }
}
QString target = ui->installerTargetCombo->currentText();
if (target == "Windows EXE") { /* existing logic */ }
else if (target == "Linux DEB") { generateDebPackage(appName, version); }
else if (target == "Linux AppImage") { generateAppImage(appName, version); }
else if (target == "macOS DMG") { generateDMG(appName, version); }
void MainWindow::openProject() {
    QString dir = QFileDialog::getExistingDirectory(this, "Open Project Folder");
    if (dir.isEmpty()) return;
    ui->repoInput->setText(dir);
    projectRoot = dir;
    ui->logOutput->append("<font color='green'>Project opened: " + dir + "</font>");

    // Load project files
    QStringList files = QDir(dir).entryList(QStringList() << "*.cpp" << "*.h", QDir::Files);
    for (const QString &file : files) {
        ui->fileListWidget->addItem(file);
        QFile::copy(dir + "/" + file, "temp/" + file);
    }
    ui->logOutput->append("<font color='green'>Project files loaded.</font>");
    ui->fileListWidget->setCurrentRow(0);
    loadFile("temp/" + ui->fileListWidget->currentItem()->text());
}
void MainWindow::onFileDoubleClicked(const QModelIndex &index) {
    if (!index.isValid()) return;
    QString filePath = fsModel->filePath(index);
    if (openEditors.contains(filePath)) {
        int tabIndex = editorTabs->indexOf(openEditors[filePath]);
        editorTabs->setCurrentIndex(tabIndex);
        return;
    }

    CodeEditor *newEditor = new CodeEditor;
    newEditor->loadFile(filePath);
    openEditors[filePath] = newEditor;

    int tabIndex = editorTabs->addTab(newEditor, QFileInfo(filePath).fileName());
    editorTabs->setCurrentIndex(tabIndex);

    connect(newEditor, &CodeEditor::modificationChanged, this, [=](bool modified) {
        QString title = QFileInfo(filePath).fileName();
        if (modified) title += "*";
        int idx = editorTabs->indexOf(newEditor);
        if (idx != -1) editorTabs->setTabText(idx, title);
    });
}
    ui->logOutput->append("<font color='green'>File opened: " + filePath + "</font>");
}
void MainWindow::closeTab(int index) {
    QWidget *widget = editorTabs->widget(index);
    if (!widget) return;

    CodeEditor *editor = qobject_cast<CodeEditor*>(widget);
    if (editor && editor->isModified()) {
        QMessageBox::StandardButton res = QMessageBox::question(this, "Unsaved Changes",
            "The file has unsaved changes. Do you want to save before closing?",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (res == QMessageBox::Yes) {
            editor->saveFile("");
        } else if (res == QMessageBox::Cancel) {
            return; // do not close
        }
    }

    QString filePath = editor->currentFile;
    openEditors.remove(filePath);
    editorTabs->removeTab(index);
    delete widget;

    ui->logOutput->append("<font color='green'>File closed: " + filePath + "</font>");
}
QTreeWidget *stackTree = new QTreeWidget;
stackTree->setHeaderLabels({"Frame", "Function"});

void MainWindow::updateStackTrace() {
    debugger->sendCommand("backtrace");

    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        stackTree->clear();
        QStringList lines = out.split("\n");
        for (const QString &line : lines) {
            if (line.startsWith("#")) {
                QStringList parts = line.split(" ");
                QTreeWidgetItem *item = new QTreeWidgetItem(stackTree);
                item->setText(0, parts[0]);
                item->setText(1, line.mid(parts[0].size()).trimmed());
                stackTree->addTopLevelItem(item);
            }
        }
    });
}

void MainWindow::openProject() {
    QString dir = QFileDialog::getExistingDirectory(this, "Open Project Folder");
    if (dir.isEmpty()) return;

    ui->repoInput->setText(dir);
    ui->logOutput->append("<font color='green'>Project opened: " + dir + "</font>");
    // Load project files
    QStringList files = QDir(dir).entryList(QStringList() << "*.cpp" << "*.h", QDir::Files);
    for (const QString &file : files) {
        ui->fileListWidget->addItem(file);
        QFile::copy(dir + "/" + file, "temp/" + file);
    }
    ui->logOutput->append("<font color='green'>Project files loaded.</font>");
    ui->fileListWidget->setCurrentRow(0);
    loadFile("temp/" + ui->fileListWidget->currentItem()->text());
}
void MainWindow::loadFile(const QString &filePath) {
    if (currentEditor) {
        delete currentEditor;
        currentEditor = nullptr;
    }
    currentEditor = new CodeEditor;
    currentEditor->loadFile(filePath);
    ui->editorLayout->addWidget(currentEditor);
}
    ui->logOutput->append("<font color='green'>File loaded: " + filePath + "</font>");

void MainWindow::onFileSelected(QListWidgetItem *item) {
    if (!item) return;
    QString filePath = "temp/" + item->text();
    loadFile(filePath);
    ui->logOutput->append("<font color='green'>File selected: " + filePath + "</font>");
}
// 🔹 Project Explorer Setup
    fsModel = new QFileSystemModel(this);
    fsModel->setRootPath(QDir::currentPath());
    fsModel->setNameFilters(QStringList() << "*.cpp" << "*.h" << "*.txt");
    fsModel->setNameFilterDisables(false);

    fileTree = new QTreeView(this);
    fileTree->setModel(fsModel);
    fileTree->setContextMenuPolicy(Qt::CustomContextMenu);

    QDockWidget *dock = new QDockWidget("Project Explorer", this);
    dock->setWidget(fileTree);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    // 🔹 Code Editor
    editor = new CodeEditor(this);
    setCentralWidget(editor);

    // 🔹 Connections
    connect(fileTree, &QTreeView::doubleClicked, this, &MainWindow::onFileDoubleClicked);
    connect(fileTree, &QWidget::customContextMenuRequested, this, [=](const QPoint &pos) {
        QModelIndex index = fileTree->indexAt(pos);
        QMenu menu;
        menu.addAction("New File", this, &MainWindow::newFile);
        if (index.isValid()) {
            menu.addAction("Delete File", this, &MainWindow::deleteFile);
            menu.addAction("Rename File", this, &MainWindow::renameFile);
        }
        menu.exec(fileTree->viewport()->mapToGlobal(pos));
    });

MainWindow::~MainWindow() {
    delete ui;
}
// 🔹 Search Panel Setup
QTreeWidget *resultsTree = nullptr;
QPlainTextEdit *memoryView = new QPlainTextEdit;

void MainWindow::updateMemory(QString addr) {
    debugger->sendCommand("x/32x " + addr);

    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        memoryView->clear();
        memoryView->appendPlainText(out);
    });
}

void MainWindow::gitDiff() {
    QProcess git;
    git.setWorkingDirectory(projectRoot);
    git.start("git", {"diff"});
    git.waitForFinished();

    QString diffOutput = git.readAllStandardOutput();

    QDialog *dlg = new QDialog(this);
    dlg->setWindowTitle("Git Diff");
    QVBoxLayout *layout = new QVBoxLayout;

    QTextEdit *diffView = new QTextEdit;
    diffView->setReadOnly(true);
    diffView->setFont(QFont("Consolas", 10));
    diffView->setText(diffOutput);

    layout->addWidget(diffView);
    dlg->setLayout(layout);
    dlg->resize(800, 600);
    dlg->exec();
}
void MainWindow::setupSearchPanel() {
    QWidget *searchPanel = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    QLineEdit *searchInput = new QLineEdit;
    QPushButton *searchBtn = new QPushButton("Search");
    resultsTree = new QTreeWidget;
    resultsTree->setHeaderLabels({"File", "Line", "Snippet"});

    layout->addWidget(new QLabel("Search in Project"));
    layout->addWidget(searchInput);
    layout->addWidget(searchBtn);
    layout->addWidget(resultsTree);
    searchPanel->setLayout(layout);

    QDockWidget *dock = new QDockWidget("Search", this);
    dock->setWidget(searchPanel);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        resultsTree->clear();
        QString query = searchInput->text();
        if (query.isEmpty()) return;

        QDirIterator it(projectRoot, QStringList() << "*.cpp" << "*.h", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            QTextStream in(&file);
            int lineNum = 1;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains(query, Qt::CaseInsensitive)) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(resultsTree);
                    item->setText(0, QFileInfo(filePath).fileName());
                    item->setText(1, QString::number(lineNum));
                    item->setText(2, line.trimmed());
                    item->setData(0, Qt::UserRole, filePath);
                }
                lineNum++;
            }
        }
    });

    connect(resultsTree, &QTreeWidget::itemDoubleClicked, this, [=](QTreeWidgetItem *item, int) {
        QString filePath = item->data(0, Qt::UserRole).toString();
        int line = item->text(1).toInt();

        if (!filePath.isEmpty()) {
            onFileDoubleClicked(fsModel->index(filePath));
            openEditors[filePath]->loadFile(filePath);
            QTextCursor cursor(openEditors[filePath]->document()->findBlockByLineNumber(line - 1));
            openEditors[filePath]->setTextCursor(cursor);
        }
    });
}

void MainWindow::onFileDoubleClicked(const QModelIndex &index) {
    QString path = fsModel->filePath(index);
    if (QFileInfo(path).isFile()) return;
    if (openEditors.contains(path)) {
        int tabIndex = editorTabs->indexOf(openEditors[path]);
        if (tabIndex != -1) {
            editorTabs->setCurrentIndex(tabIndex);
            return;
        // Already open?
    if (openEditors.contains(path)) {
        editorTabs->setCurrentWidget(openEditors[path]);
        return;
    }
        }
    }
}
CodeEditor *editor = new CodeEditor;
    editor->loadFile(path);

    int tabIndex = editorTabs->addTab(editor, QFileInfo(path).fileName());
    editorTabs->setCurrentIndex(tabIndex);

    openEditors[path] = editor;

void MainWindow::newFile() {
    QString name = QInputDialog::getText(this, "New File", "Enter filename:");
    if (!name.isEmpty()) {
        QString newPath = projectRoot + "/" + name;
        QFile file(newPath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write("// New file created by Shuriken\n");
            file.close();
        }
        fsModel->refresh();
    }
}

void MainWindow::closeTab(int index) {
    QWidget *widget = editorTabs->widget(index);
    if (!widget) return;

    QString filePath;
    for (auto it = openEditors.begin(); it != openEditors.end(); ++it) {
        if (it.value() == widget) {
            filePath = it.key();
            break;
        }
    }

    if (!filePath.isEmpty()) openEditors.remove(filePath);
    editorTabs->removeTab(index);
    widget->deleteLater();
}

void MainWindow::deleteFile() {
    QModelIndex index = fileTree->currentIndex();
    if (!index.isValid()) return;

    QString path = fsModel->filePath(index);
    if (QMessageBox::question(this, "Delete File", "Delete " + path + "?") == QMessageBox::Yes) {
        QFile::remove(path);
        fsModel->refresh();
    }
}

void MainWindow::startDebug() {
    if (builtExe.isEmpty()) {
        QMessageBox::warning(this, "Debug", "No executable built.");
        return;
    }

    debugger = new Debugger(builtExe, this);
    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        ui->logOutput->append("<font color='blue'>[Debug] " + out + "</font>");
    });

    debugger->start();
    debugger->sendCommand("break main");
    debugger->sendCommand("run");
}
void MainWindow::updateVariables() {
    debugger->sendCommand("info locals");
    debugger->sendCommand("info args");

    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        QStringList lines = out.split("\n");
        for (const QString &line : lines) {
            if (line.contains("=")) {
                QStringList parts = line.split("=");
                if (parts.size() == 2) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(watchTree);
                    item->setText(0, parts[0].trimmed());
                    item->setText(1, parts[1].trimmed());
                    watchTree->addTopLevelItem(item);
                }
            }
        }
    });
}

void MainWindow::renameFile() {
    QModelIndex index = fileTree->currentIndex();
    if (!index.isValid()) return;

    QString path = fsModel->filePath(index);
    QString newName = QInputDialog::getText(this, "Rename File", "New name:");
    if (!newName.isEmpty()) {
        QFileInfo fi(path);
        QString newPath = fi.absolutePath() + "/" + newName;
        QFile::rename(path, newPath);
        fsModel->refresh();
    }
}
Updater *updater = new Updater(this);
connect(updater, &Updater::updateAvailable, this, [=](QString v) {
    QMessageBox::information(this, "Update Available",
                             "A new version (" + v + ") is available. Download from GitHub?");
});
updater->checkForUpdates();
Updater *updater = new Updater(this);
connect(updater, &Updater::updateAvailable, this, [=](QString v) {
    QMessageBox::information(this, "Update Available",
                             "A new version (" + v + ") is available. Download from GitHub?");
});
void MainWindow::renameSymbol(QString file, int line, int col, QString newName) {
    CXTranslationUnit tu = clang_parseTranslationUnit(globalCompleter->index,
        file.toUtf8().data(), nullptr, 0, nullptr, 0, CXTranslationUnit_None);

    CXFile cxFile = clang_getFile(tu, file.toUtf8().data());
    CXSourceLocation loc = clang_getLocation(tu, cxFile, line, col);
    CXCursor cursor = clang_getCursor(tu, loc);

    CXString oldName = clang_getCursorSpelling(cursor);
    QString old = QString::fromUtf8(clang_getCString(oldName));

    // Replace occurrences in file
    QFile f(file);
    if (f.open(QIODevice::ReadOnly)) {
        QString code = f.readAll();
        code.replace(QRegExp("\\b" + old + "\\b"), newName);
        f.close();

        if (f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            f.write(code.toUtf8());
            f.close();
        }
    }
    clang_disposeTranslationUnit(tu);
}
updater->checkForUpdates();
connect(updater, &Updater::noUpdateAvailable, this, [=]() {
    ui->logOutput->append("<font color='green'>No updates available.</font>");
});
void MainWindow::formatFile(QString file) {
    QProcess clangFmt;
    clangFmt.start("clang-format", {"-i", file});
    clangFmt.waitForFinished();
    ui->logOutput->append("Formatted " + file);
}
void MainWindow::formatFile(QString file) {
    QProcess clangFmt;
    clangFmt.start("clang-format", {"-i", file});
    clangFmt.waitForFinished();
    ui->logOutput->append("Formatted " + file);
}
    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
void MainWindow::installPlugin(QString repoUrl) {
    QProcess git;
    git.start("git", {"clone", repoUrl, "plugins/"});
    git.waitForFinished();
    ui->logOutput->append("Installed plugin from " + repoUrl);
}
QDockWidget *chatDock = new QDockWidget("Collaboration Chat", this);
QTextEdit *chatLog = new QTextEdit;
chatLog->setReadOnly(true);
QLineEdit *chatInput = new QLineEdit;

connect(chatInput, &QLineEdit::returnPressed, this, [=]() {
    socket->sendTextMessage(chatInput->text());
    chatLog->append("<b>You:</b> " + chatInput->text());
    chatInput->clear();
});
QVBoxLayout *chatLayout = new QVBoxLayout;
chatLayout->addWidget(chatLog);
chatLayout->addWidget(chatInput);
chatDock->setWidget(new QWidget);
chatDock->widget()->setLayout(chatLayout);
addDockWidget(Qt::RightDockWidgetArea, chatDock);

    connect(socket, &QWebSocket::textMessageReceived, this, [=](QString msg) {
        chatLog->append("<b>Peer:</b> " + msg);
    });

    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
    updater->checkForUpdates();
    startCollaboration();
    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
    void MainWindow::exportCICD(QString target) {
    QString file;
    if (target == "GitHub") file = ".github/workflows/build.yml";
    else if (target == "GitLab") file = ".gitlab-ci.yml";
    else if (target == "Jenkins") file = "Jenkinsfile";

    QFile out(projectRoot + "/" + file);
    out.open(QIODevice::WriteOnly);
    out.write(generatePipelineYaml(target).toUtf8());
    out.close();

    ui->logOutput->append("Exported CI/CD pipeline for " + target);
}
QDockWidget *dashboardDock = new QDockWidget("Team Dashboard", this);
QSplitter *split = new QSplitter(Qt::Horizontal);

QListWidget *todo = new QListWidget;
QListWidget *progress = new QListWidget;
QListWidget *done = new QListWidget;

todo->addItem("Refactor Builder.cpp");
progress->addItem("Implement IntelliSense");
done->addItem("Add Installer system");

split->addWidget(todo);
split->addWidget(progress);
split->addWidget(done);

dashboardDock->setWidget(split);
addDockWidget(Qt::RightDockWidgetArea, dashboardDock);

void MainWindow::saveSecret(QString key, QString value) {
    QByteArray hash = QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha256);
    QSettings settings("Shuriken", "Secrets");
    settings.setValue(key, hash.toHex());
}
QString MainWindow::loadSecret(QString key) {
    QSettings settings("Shuriken", "Secrets");
    return settings.value(key).toString();
}
void MainWindow::pushArtifact(QString path, QString registry) {
    QProcess docker;
    docker.start("docker", {"push", registry + "/" + path});
    docker.waitForFinished();
    ui->logOutput->append("Pushed artifact to " + registry);
}
    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
    updater->checkForUpdates();
QDockWidget *aiDock = new QDockWidget("AI Assistant", this);
QTextEdit *aiLog = new QTextEdit;
aiLog->setReadOnly(true);
QLineEdit *aiInput = new QLineEdit;

connect(aiInput, &QLineEdit::returnPressed, this, [=]() {
    QJsonObject req;
    req["prompt"] = aiInput->text();
    QNetworkReply *r = manager->post(QNetworkRequest(QUrl("https://shuriken-ai.example.com/chat")),
                                     QJsonDocument(req).toJson());
    connect(r, &QNetworkReply::finished, this, [=]() {
        aiLog->append("<b>You:</b> " + aiInput->text());
        aiLog->append("<b>AI:</b> " + r->readAll());
    });
    aiInput->clear();
});
QVBoxLayout *aiLayout = new QVBoxLayout;
aiLayout->addWidget(aiLog);
aiLayout->addWidget(aiInput);
aiDock->setWidget(new QWidget);
aiDock->widget()->setLayout(aiLayout);
addDockWidget(Qt::RightDockWidgetArea, aiDock);
    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
    updater->checkForUpdates();
    startCollaboration();
    setupSearchPanel();
    setupTerminalPanel();
    setupDashboard();
    setupAIChat();
}
QTreeWidget *propertyTree = new QTreeWidget;
propertyTree->setHeaderLabels({"Property", "Value"});

// Example: show QPushButton props
QTreeWidgetItem *textProp = new QTreeWidgetItem(propertyTree);
textProp->setText(0, "Text");
textProp->setText(1, "Click Me");

connect(propertyTree, &QTreeWidget::itemChanged, this, [=](QTreeWidgetItem *item, int col) {
    if (item->text(0) == "Text") {
        selectedWidget->setProperty("text", item->text(1));
    }
});
QDockWidget *propDock = new QDockWidget("Properties", this);
propDock->setWidget(propertyTree);
addDockWidget(Qt::RightDockWidgetArea, propDock);
void MainWindow::showFileContextMenu(const QPoint &pos) {
    QModelIndex index = fileTree->indexAt(pos);
    if (!index.isValid()) return;

    QString path = fsModel->filePath(index);
    QMenu menu;
    menu.addAction("Open", this, [=]() { onFileDoubleClicked(index); });
    menu.addAction("Delete", this, &MainWindow::deleteFile);
    menu.addAction("Rename", this, &MainWindow::renameFile);
    menu.addAction("Format", this, [=]() { formatFile(path); });
    menu.exec(fileTree->viewport()->mapToGlobal(pos));
}
    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
    updater->checkForUpdates();
    startCollaboration();
    setupSearchPanel();
    setupTerminalPanel();
    setupDashboard();
    setupAIChat();
    setupPropertyEditor();
    connect(fileTree, &QWidget::customContextMenuRequested, this, &MainWindow::showFileContextMenu);
}
    addDockWidget(Qt::RightDockWidgetArea, propDock);
}
void MainWindow::setupPropertyEditor() {
    QTreeWidget *propertyTree = new QTreeWidget;
    propertyTree->setHeaderLabels({"Property", "Value"});

    // Example: show QPushButton props
    QTreeWidgetItem *textProp = new QTreeWidgetItem(propertyTree);
    textProp->setText(0, "Text");
    textProp->setText(1, "Click Me");

    connect(propertyTree, &QTreeWidget::itemChanged, this, [=](QTreeWidgetItem *item, int col) {
        if (item->text(0) == "Text") {
            selectedWidget->setProperty("text", item->text(1));
        }
    });

    QDockWidget *propDock = new QDockWidget("Properties", this);
    propDock->setWidget(propertyTree);
    addDockWidget(Qt::RightDockWidgetArea, propDock);
}
QDockWidget *marketDock = new QDockWidget("Marketplace", this);
QListWidget *marketList = new QListWidget;

marketList->addItem("Plugin: Clang-Tidy");
marketList->addItem("Template: Game Engine Starter");
marketList->addItem("UI Pack: Material Dark Widgets");

connect(marketList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
    QString selection = item->text();
    if (selection.startsWith("Plugin")) installPlugin(selection);
    else if (selection.startsWith("Template")) installTemplate(selection);
    else if (selection.startsWith("UI Pack")) installUIPack(selection);
});
marketDock->setWidget(marketList);
addDockWidget(Qt::RightDockWidgetArea, marketDock);
void MainWindow::setupMarketplace() {
    QDockWidget *marketDock = new QDockWidget("Marketplace", this);
    QListWidget *marketList = new QListWidget;

    marketList->addItem("Plugin: Clang-Tidy");
    marketList->addItem("Template: Game Engine Starter");
    marketList->addItem("UI Pack: Material Dark Widgets");

    connect(marketList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
        QString selection = item->text();
        if (selection.startsWith("Plugin")) installPlugin(selection);
        else if (selection.startsWith("Template")) installTemplate(selection);
        else if (selection.startsWith("UI Pack")) installUIPack(selection);
    });

    marketDock->setWidget(marketList);
    addDockWidget(Qt::RightDockWidgetArea, marketDock);
}
void MainWindow::gitCommit() {
    bool ok;
    QString msg = QInputDialog::getText(this, "Git Commit", "Enter commit message:", QLineEdit::Normal, "", &ok);
    if (!ok || msg.isEmpty()) return;

    QProcess git;
    git.setWorkingDirectory(projectRoot);
    git.start("git", {"add", "."});
    git.waitForFinished();

    git.start("git", {"commit", "-m", msg});
    git.waitForFinished();
    gitLog->appendPlainText(git.readAllStandardOutput());
}
MainWindow::~MainWindow() {
    delete ui;
    delete fsModel;
    delete fileTree;
    delete debugger;
    delete builderProcess;
    delete watchTree;
    delete stackTree;
    delete memoryView;
    delete socket;
    delete manager;
    delete updater;
    delete globalCompleter;
    qDeleteAll(openEditors);
}
QDockWidget *profileDock = new QDockWidget("User Profile", this);
QLabel *avatar = new QLabel;
avatar->setPixmap(QPixmap("resources/avatar.png").scaled(64,64));
QLabel *username = new QLabel("Signed in as: Guest");

QVBoxLayout *layout = new QVBoxLayout;
layout->addWidget(avatar);
layout->addWidget(username);
QWidget *profileWidget = new QWidget;
profileWidget->setLayout(layout);
profileDock->setWidget(profileWidget);
addDockWidget(Qt::LeftDockWidgetArea, profileDock);
void MainWindow::displayLog(const QString &log) {
    ui->logOutput->clear();
    QStringList lines = log.split("\n");
    for (const QString &line : lines) {
        if (line.contains("error", Qt::CaseInsensitive)) {
            ui->logOutput->appendHtml("<font color='red'>" + line + "</font>");
        } else {
            ui->logOutput->append(line);
        }
    }
}
void MainWindow::setupLogOutput() {
    ui->logOutput->setReadOnly(true);
    ui->logOutput->setFont(QFont("Consolas", 10));
    ui->logOutput->clear();

    // Example log entries
    QStringList logLines = {
        "Compiling main.cpp...",
        "main.cpp:10:5: warning: unused variable 'x' [-Wunused-variable]",
        "main.cpp:15:10: error: 'y' was not declared in this scope",
        "Linking...",
        "Build successful."
    };

    for (const QString &line : logLines) {
        if (line.contains("error", Qt::CaseInsensitive)) {
            ui->logOutput->appendHtml("<font color='red'>" + line + "</font>");
        } else if (line.contains("warning", Qt::CaseInsensitive)) {
            ui->logOutput->appendHtml("<font color='orange'>" + line + "</font>");
        } else {
            ui->logOutput->append(line);
        }
    }
}
void MainWindow::onLogLinkClicked(const QUrl &link) {
    QString url = link.toString();
    QRegExp fileLineRegex(R"(file://(.+):(\d+))");
    if (fileLineRegex.indexIn(url) != -1) {
        QString filePath = fileLineRegex.cap(1);
        int lineNumber = fileLineRegex.cap(2).toInt();

        // Open file in editor at specific line
        if (openEditors.contains(filePath)) {
            int tabIndex = editorTabs->indexOf(openEditors[filePath]);
            if (tabIndex != -1) {
                editorTabs->setCurrentIndex(tabIndex);
                QTextCursor cursor(openEditors[filePath]->document()->findBlockByLineNumber(lineNumber - 1));
                openEditors[filePath]->setTextCursor(cursor);
                return;
            }
        }

        // If not already open, open new tab
        CodeEditor *newEditor = new CodeEditor;
        newEditor->loadFile(filePath);
        openEditors[filePath] = newEditor;

        int tabIndex = editorTabs->addTab(newEditor, QFileInfo(filePath).fileName());
        editorTabs->setCurrentIndex(tabIndex);

        QTextCursor cursor(newEditor->document()->findBlockByLineNumber(lineNumber - 1));
        newEditor->setTextCursor(cursor);
    } else {
        // Handle other links (e.g., URLs)
        QDesktopServices::openUrl(link);
    }
}
void MainWindow::displayLog(const QString &log) {
    ui->logOutput->clear();
    QStringList lines = log.split("\n");
    for (const QString &line : lines) {
        if (line.contains("error", Qt::CaseInsensitive)) {
            ui->logOutput->appendHtml("<a href='file://" + line + "'><font color='red'>" + line + "</font></a>");
        } else if (line.contains("warning", Qt::CaseInsensitive)) {
            ui->logOutput->appendHtml("<font color='orange'>" + line + "</font>");
        } else {
            ui->logOutput->append(line);
        }
    }
    connect(ui->logOutput, &QTextBrowser::anchorClicked, this, &MainWindow::onLogLinkClicked);
}
void MainWindow::packageInstaller() {
    if (builtExe.isEmpty()) {
        QMessageBox::warning(this, "Package Installer", "No executable built.");
        return;
    }

    QString appName = ui->appNameInput->text().isEmpty() ? "MyApp" : ui->appNameInput->text();
    QString version = ui->versionInput->text().isEmpty() ? "1.0.0" : ui->versionInput->text();
    QString outInstaller = projectRoot + "/" + appName + "_v" + version + ".exe";
    QString target = ui->installerTargetCombo->currentText();
    ui->progressBar->setValue(0);
    ui->statusBar->showMessage("Packaging installer...");
    ui->installerLogOutput->clear();
    ui->installerLogOutput->append("Packaging installer for " + target + "...");
    if (target == "Windows EXE") {
        QProcess makensis;
        QString nsisScript = "installer_script.nsi";
        QFile scriptFile(nsisScript);
        if (scriptFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&scriptFile);
            out << "OutFile \"" << outInstaller << "\"\n";
            out << "InstallDir \"$PROGRAMFILES\\" << appName << "\"\n";
            out << "Page directory\n";
            out << "Page instfiles\n";
            out << "Section \"Install\"\n";
            out << "SetOutPath \"$INSTDIR\"\n";
            out << "File \"" << builtExe << "\"\n";
            out << "CreateShortCut \"$DESKTOP\\" << appName << ".lnk\" \"$INSTDIR\\" << QFileInfo(builtExe).fileName() << "\"\n";
            out << "SectionEnd\n";
            scriptFile.close();
        }

        makensis.start("makensis", {nsisScript});
        connect(makensis, &QProcess::readyReadStandardOutput, this, [=]() {
            QString output = makensis.readAllStandardOutput();
            ui->installerLogOutput->append(output);
        });
        connect(makensis, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus) {
            if (exitCode == 0) {
                ui->installerLogOutput->append("<font color='green'>Installer created: " + outInstaller + "</font>");
                ui->progressBar->setValue(100);
                ui->statusBar->showMessage("Packaging complete");
                QMessageBox::information(this, "Packaging Complete", "Installer created successfully.");
            } else {
                ui->installerLogOutput->append("<font color='red'>Failed to create installer.</font>");
                ui->statusBar->showMessage("Packaging failed");
                ui->progressBar->setValue(0);
            }
        });
        makensis.start();
    } else if (target == "Linux DEB") {
        generateDebPackage(appName, version);
    } else if (target == "Linux AppImage") {
        generateAppImage(appName, version);
    } else if (target == "macOS DMG") {
        generateDMG(appName, version);
    }
}
void MainWindow::generateDebPackage(const QString &appName, const QString &version) {
    QString debDir = projectRoot + "/" + appName + "_deb";
    QDir().mkpath(debDir + "/DEBIAN");
    QDir().mkpath(debDir + "/usr/local/bin");

    QFile controlFile(debDir + "/DEBIAN/control");
    if (controlFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&controlFile);
        out << "Package: " << appName.toLower() << "\n";
        out << "Version: " << version << "\n";
        out << "Section: base\n";
        out << "Priority: optional\n";
        out << "Architecture: amd64\n";
        out << "Maintainer: Your Name <youremail@example.com>\n";
        out << "Description: A brief description of your package.\n";
        controlFile.close();
    }

    QFile binaryFile(debDir + "/usr/local/bin/" + appName.toLower());
    if (binaryFile.open(QIODevice::WriteOnly)) {
        binaryFile.write("#!/bin/bash\n");
        binaryFile.write("exec " + builtExe + " \"$@\"\n");
        binaryFile.close();
    }

    QProcess dpkgBuild;
    dpkgBuild.start("dpkg-deb", {"--build", debDir});
    connect(dpkgBuild, &QProcess::finished, this, [=](int exitCode, QProcess::ExitStatus) {
        if (exitCode == 0) {
            ui->installerLogOutput->append("<font color='green'>DEB package created successfully.</font>");
        } else {
            ui->installerLogOutput->append("<font color='red'>Failed to create DEB package.</font>");
        }
    });
}
QDialog *loginDialog = new QDialog;
QLineEdit *email = new QLineEdit;
QLineEdit *password = new QLineEdit;
password->setEchoMode(QLineEdit::Password);
QPushButton *loginBtn = new QPushButton("Sign In");

connect(loginBtn, &QPushButton::clicked, this, [=]() {
    QJsonObject req;
    req["email"] = email->text();
    req["password"] = password->text();

    QNetworkRequest request(QUrl("https://shuriken-auth.example.com/login"));
    QNetworkReply *r = manager->post(request, QJsonDocument(req).toJson());
    connect(r, &QNetworkReply::finished, this, [=]() {
        QJsonObject resp = QJsonDocument::fromJson(r->readAll()).object();
        if (resp["success"].toBool()) {
            username->setText("Signed in as: " + resp["username"].toString());
        }
    });
});
QVBoxLayout *layout = new QVBoxLayout;
layout->addWidget(new QLabel("Email:"));
layout->addWidget(email);
layout->addWidget(new QLabel("Password:"));
layout->addWidget(password);
layout->addWidget(loginBtn);
loginDialog->setLayout(layout);
loginDialog->exec();
void MainWindow::closeTab(int index) {
    QWidget *widget = editorTabs->widget(index);
    if (!widget) return;

    CodeEditor *editor = qobject_cast<CodeEditor*>(widget);
    if (editor && editor->isModified()) {
        int res = QMessageBox::question(this, "Unsaved Changes",
                                        "The file has unsaved changes. Save before closing?",
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (res == QMessageBox::Cancel) return;
        if (res == QMessageBox::Yes) editor->saveFile();
    }
    QString filePath;
    for (auto it = openEditors.begin(); it != openEditors.end(); ++it) {
        if (it.value() == widget) {
            filePath = it.key();
            break;
        }
    }
    void MainWindow::syncSettings() {
    QFile settings("settings.json");
    if (!settings.open(QIODevice::ReadOnly)) return;

    QNetworkRequest req(QUrl("https://shuriken-cloud.example.com/sync/settings"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray data = settings.readAll();
    manager->post(req, data);
}
    if (!filePath.isEmpty()) openEditors.remove(filePath);
    editorTabs->removeTab(index);
    widget->deleteLater();
}
void MainWindow::updateStack() {
    debugger->sendCommand("backtrace");

    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        stackTree->clear();
        QStringList lines = out.split("\n");
        for (const QString &line : lines) {
            if (line.startsWith("#")) {
                QTreeWidgetItem *item = new QTreeWidgetItem(stackTree);
                item->setText(0, line.section(" ", 1, 1).trimmed());
                item->setText(1, line.section(" ", 2).section(" at ", 0, 0).trimmed());
                item->setText(2, line.section(" at ", 1).section(":", 0, 0).trimmed());
                item->setText(3, line.section(":", -1).section(")", 0, 0).remove(")").simmed());
                stackTree->addTopLevelItem(item);
            }
        }
    });
}
    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        QStringList lines = out.split("\n");
        for (const QString &line : lines) {
            if (line.startsWith("#")) {
                QTreeWidgetItem *item = new QTreeWidgetItem(stackTree);
                item->setText(0, line.section(" ", 1, 1).trimmed());
                item->setText(1, line.section(" ", 2).section(" at ", 0, 0).trimmed());
                item->setText(2, line.section(" at ", 1).section(":", 0, 0).trimmed());
                item->setText(3, line.section(":", -1).section(")", 0, 0).remove(")").trimmed());
                stackTree->addTopLevelItem(item);
            }
        }
    });
}
    if (!filePath.isEmpty()) openEditors.remove(filePath);
    editorTabs->removeTab(index);
    widget->deleteLater();
}
void MainWindow::syncProject(QString projectPath) {
    QProcess zip;
    zip.start("zip", {"-r", "project.zip", projectPath});
    zip.waitForFinished();

    QFile file("project.zip");
    QNetworkRequest req(QUrl("https://shuriken-cloud.example.com/sync/project"));
    manager->post(req, file.readAll());
}
QTableWidget *taskTable = new QTableWidget(0, 4);
taskTable->setHorizontalHeaderLabels({"ID", "Title", "Assignee", "Status"});

void MainWindow::addTask(QString title, QString assignee) {
    int row = taskTable->rowCount();
    taskTable->insertRow(row);
    taskTable->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));
    taskTable->setItem(row, 1, new QTableWidgetItem(title));
    taskTable->setItem(row, 2, new QTableWidgetItem(assignee));
    taskTable->setItem(row, 3, new QTableWidgetItem("To Do"));
}
connect(fileTree, &QTreeView::customContextMenuRequested, this, [=](const QPoint &pos) {
        QModelIndex index = fileTree->indexAt(pos);
        if (!index.isValid()) return;

        QString path = fsModel->filePath(index);
        QMenu menu;
        if (QFileInfo(path).isDir()) {
            menu.addAction("New File", this, &MainWindow::newFile);
            menu.addAction("New Folder", this, &MainWindow::newFolder);
        } else {
            menu.addAction("Open", this, [=]() { onFileDoubleClicked(index); });
            menu.addAction("Delete File", this, &MainWindow::deleteFile);
            menu.addAction("Rename File", this, &MainWindow::renameFile);
            menu.addAction("Format File", this, [=]() { formatFile(path); });
            menu.addAction("Git Diff", this, &MainWindow::showGitDiff);
        }
        menu.exec(fileTree->viewport()->mapToGlobal(pos));
    });
    void MainWindow::generateTests(QString file) {
    QFile source(file);
    source.open(QIODevice::ReadOnly);
    QString code = source.readAll();

    QJsonObject req;
    req["source"] = code;

    QNetworkReply *r = manager->post(
        QNetworkRequest(QUrl("https://shuriken-ai.example.com/tests")),
        QJsonDocument(req).toJson()
    );

    connect(r, &QNetworkReply::finished, this, [=]() {
        QString tests = QString::fromUtf8(r->readAll());
        QFile t("tests_generated.cpp");
        t.open(QIODevice::WriteOnly);
        t.write(tests.toUtf8());
        ui->logOutput->append("AI-generated tests written to tests_generated.cpp");
    });
}
void MainWindow::setupSearchPanel() {
    QWidget *searchPanel = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    QLineEdit *searchInput = new QLineEdit;
    searchInput->setPlaceholderText("Enter search query...");
    QPushButton *searchBtn = new QPushButton("Search");
    QTreeWidget *resultsTree = new QTreeWidget;
    resultsTree->setHeaderLabels({"File", "Line", "Snippet"});

    layout->addWidget(searchInput);
    layout->addWidget(searchBtn);
   layout->addWidget(resultsTree);
   searchPanel->setLayout(layout);
   ui->mainLayout->addWidget(searchPanel);
}

    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        resultsTree->clear();
        QString query = searchInput->text();
        if (query.isEmpty()) return;

        QDirIterator it(projectRoot, QStringList() << "*.cpp" << "*.h", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            QTextStream in(&file);
            int lineNum = 1;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains(query, Qt::CaseInsensitive)) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(resultsTree);
                    item->setText(0, QFileInfo(filePath).fileName());
                    item->setText(1, QString::number(lineNum));
                    item->setText(2, line.trimmed());
                    resultsTree->addTopLevelItem(item);
                }
                lineNum++;
            }
            file.close();
        }
    });
    void MainWindow::runTests() {
    QProcess testProc;
    testProc.start("ctest");
    testProc.waitForFinished();
    ui->logOutput->append(testProc.readAllStandardOutput());
}
    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        resultsTree->clear();
        QString query = searchInput->text();
        if (query.isEmpty()) return;

        QDirIterator it(projectRoot, QStringList() << "*.cpp" << "*.h", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            QTextStream in(&file);
            int lineNum = 1;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains(query, Qt::CaseInsensitive)) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(resultsTree);
                    item->setText(0, QFileInfo(filePath).fileName());
                    item->setText(1, QString::number(lineNum));
                    item->setText(2, line.trimmed());
                    resultsTree->addTopLevelItem(item);
                }
                lineNum++;
            }
            file.close();
        }
    });
    void MainWindow::generateUML(QString projectRoot) {
    QProcess proc;
    proc.start("clang-uml", {"-o", "diagram.puml", projectRoot});
    proc.waitForFinished();

    QProcess render;
    render.start("plantuml", {"diagram.puml"});
    render.waitForFinished();

    ui->logOutput->append("Generated UML diagram: diagram.png");
}
    connect(searchBtn, &QPushButton::clicked, this, [=]() {
        resultsTree->clear();
        QString query = searchInput->text();
        if (query.isEmpty()) return;

        QDirIterator it(projectRoot, QStringList() << "*.cpp" << "*.h", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            QTextStream in(&file);
            int lineNum = 1;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.contains(query, Qt::CaseInsensitive)) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(resultsTree);
                    item->setText(0, QFileInfo(filePath).fileName());
                    item->setText(1, QString::number(lineNum));
                    item->setText(2, line.trimmed());
                    resultsTree->addTopLevelItem(item);
                }
                lineNum++;
            }
            file.close();
        }
    });
}
void MainWindow::generateRoadmap() {
    QFile code("src/");
    QJsonObject req;
    req["code"] = "entire project";

    QNetworkReply *r = manager->post(
        QNetworkRequest(QUrl("https://shuriken-ai.example.com/roadmap")),
        QJsonDocument(req).toJson()
    );

    connect(r, &QNetworkReply::finished, this, [=]() {
        ui->logOutput->append("AI Roadmap:\n" + r->readAll());
    });
}
void MainWindow::updateWatches() {
    debugger->sendCommand("info locals");

    connect(debugger, &Debugger::debuggerOutput, this, [=](QString out) {
        watchTree->clear();
        QStringList lines = out.split("\n");
        for (const QString &line : lines) {
            if (line.contains("=")) {
                QStringList parts = line.split("=");
                if (parts.size() == 2) {
                    QTreeWidgetItem *item = new QTreeWidgetItem(watchTree);
                    item->setText(0, parts[0].trimmed());
                    item->setText(1, parts[1].trimmed());
                    watchTree->addTopLevelItem(item);
                }
            }
        }
    });
}
void MainWindow::showPipelineLogs() {
    QProcess proc;
    proc.start("gh", {"run", "list"});
    proc.waitForFinished();
    ui->logOutput->append(proc.readAllStandardOutput());
}
    connect(updater, &Updater::noUpdateAvailable, this, [=]() {
        ui->logOutput->append("<font color='green'>No updates available.</font>");
    });
    void MainWindow::searchPackage(QString name) {
    QProcess proc;
    proc.start("vcpkg", {"search", name});
    proc.waitForFinished();
    ui->logOutput->append(proc.readAllStandardOutput());
}
void MainWindow::installPackage(QString name) {
    QProcess proc;
    proc.start("vcpkg", {"install", name});
    proc.waitForFinished();
    ui->logOutput->append("Installed " + name);
}
void MainWindow::renameSymbol(QString file, int line, int col, QString newName) {
    CXIndex index = clang_createIndex(0, 0);
    const char *args[] = {"-I/usr/include", "-I/usr/local/include"};
    CXTranslationUnit tu = clang_parseTranslationUnit(index, file.toUtf8().data(), args, 2, nullptr, 0, CXTranslationUnit_None);
    if (!tu) {
        ui->logOutput->append("<font color='red'>Failed to parse translation unit.</font>");
        return;
    }

    CXFile cxFile = clang_getFileFromFilenames(tu, file.toUtf8().data());
    CXSourceLocation loc = clang_getLocation(tu, cxFile, line, col);
    CXCursor cursor = clang_getCursor(tu, loc);
    if (clang_Cursor_isNull(cursor)) {
        ui->logOutput->append("<font color='red'>No symbol found at location.</font>");
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
        return;
    }
    void MainWindow::runInContainer(QString image, QString appPath) {
    QProcess docker;
    docker.start("docker", {"run", "--rm", "-v", appPath + ":/app", image, "/app/MyApp"});
    docker.waitForFinished();
    ui->logOutput->append(docker.readAllStandardOutput());
}
    CXCursorKind kind = clang_getCursorKind(cursor);
    if (kind != CXCursor_VarDecl && kind != CXCursor_FunctionDecl && kind != CXCursor_CXXMethod) {
        ui->logOutput->append("<font color='red'>Symbol is not a variable or function.</font>");
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
        return;
    }

    CXSourceRange range = clang_getCursorExtent(cursor);
    CXFileRange fileRange = clang_getFileRange(tu, range);
    unsigned startLine, startCol, endLine, endCol;
    clang_getSpellingLocation(clang_getRangeStart(fileRange), nullptr, &startLine, &startCol, nullptr);
    clang_getSpellingLocation(clang_getRangeEnd(fileRange), nullptr, &endLine, &endCol, nullptr);

    QFile srcFile(file);
    if (!srcFile.open(QIODevice::ReadOnly)) {
        ui->logOutput->append("<font color='red'>Failed to open source file.</font>");
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
        return;
    }
    QString content = srcFile.readAll();
    srcFile.close();

    QStringList lines = content.split("\n");
    if (startLine > lines.size() || endLine > lines.size()) {
        ui->logOutput->append("<font color='red'>Invalid line numbers.</font>");
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
        return;
    }

    QStringList affectedLines = lines.mid(startLine - 1, endLine - startLine + 1);
    affectedLines[0].replace(startCol - 1, affectedLines[0].length() - (startCol - 1), newName);
    if (startLine != endLine) {
        affectedLines[affectedLines.size() - 1].replace(0, endCol - 1, newName);
    }

    for (int i = 0; i < affectedLines.size(); ++i) {
        lines[startLine - 1 + i] = affectedLines[i];
    }

    if (!srcFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        ui->logOutput->append("<font color='red'>Failed to write to source file.</font>");
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);
        return;
    }
    srcFile.write(lines.join("\n").toUtf8());
    srcFile.close();
    ui->logOutput->append("Renamed symbol to " + newName);
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
}
void MainWindow::runInVM(QString isoPath) {
    QProcess qemu;
    qemu.start("qemu-system-x86_64", {"-cdrom", isoPath, "-m", "2048"});
    qemu.waitForStarted();
    ui->logOutput->append("Running VM with ISO: " + isoPath);
}
QDockWidget *dashboardDock = new QDockWidget("Dashboard", this);
QWidget *dashboardWidget = new QWidget;
dashboardDock->setWidget(dashboardWidget);
addDockWidget(Qt::RightDockWidgetArea, dashboardDock);
QVBoxLayout *layout = new QVBoxLayout;
QLabel *welcome = new QLabel("<h2>Welcome to Shuriken IDE</h2>");
QLabel *stats = new QLabel("Projects: 5 | Open Files: 12 | Active Debug Sessions: 1");
QPushButton *newProjBtn = new QPushButton("New Project");
QPushButton *openProjBtn = new QPushButton("Open Project");
QPushButton *recentBtn = new QPushButton("Recent Projects");
layout->addWidget(welcome);
layout->addWidget(stats);
layout->addWidget(newProjBtn);
layout->addWidget(openProjBtn);
layout->addWidget(recentBtn);
dashboardWidget->setLayout(layout);
void MainWindow::setupDashboard() {
    QDockWidget *dashboardDock = new QDockWidget("Dashboard", this);
    QWidget *dashboardWidget = new QWidget;
    dashboardDock->setWidget(dashboardWidget);
    addDockWidget(Qt::RightDockWidgetArea, dashboardDock);

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *welcome = new QLabel("<h2>Welcome to Shuriken IDE</h2>");
    QLabel *stats = new QLabel("Projects: 5 | Open Files: 12 | Active Debug Sessions: 1");
    QPushButton *newProjBtn = new QPushButton("New Project");
    QPushButton *openProjBtn = new QPushButton("Open Project");
    QPushButton *recentBtn = new QPushButton("Recent Projects");

    layout->addWidget(welcome);
    layout->addWidget(stats);
    layout->addWidget(newProjBtn);
    layout->addWidget(openProjBtn);
    layout->addWidget(recentBtn);
    dashboardWidget->setLayout(layout);
}
void MainWindow::setupAIChat() {
    QDockWidget *aiDock = new QDockWidget("AI Chat", this);
    QTextEdit *aiLog = new QTextEdit;
    aiLog->setReadOnly(true);
    aiLog->setFont(QFont("Consolas", 10));

    QLineEdit *aiInput = new QLineEdit;
    aiInput->setPlaceholderText("Ask AI for code help...");

    connect(aiInput, &QLineEdit::returnPressed, this, [=]() {
        QString question = aiInput->text();
        if (question.isEmpty()) return;

        aiLog->append("<b>You:</b> " + question);
        aiInput->clear();

        QJsonObject req;
        req["question"] = question;

        QNetworkReply *r = manager->post(
            QNetworkRequest(QUrl("https://shuriken-ai.example.com/chat")),
            QJsonDocument(req).toJson()
        );

        connect(r, &QNetworkReply::finished, this, [=]() {
            QString answer = QString::fromUtf8(r->readAll());
            aiLog->append("<b>AI:</b> " + answer);
        });
    });

    QVBoxLayout *aiLayout = new QVBoxLayout;
    aiLayout->addWidget(aiLog);
   aiLayout->addWidget(aiInput);
   aiDock->setLayout(aiLayout);
   addDockWidget(Qt::BottomDockWidgetArea, aiDock);
}
void MainWindow::loadBugStats() {
    QProcess git;
    git.start("git", {"log", "--grep", "fix", "--pretty=oneline"});
    git.waitForFinished();
    ui->logOutput->append("Bugs Fixed:\n" + git.readAllStandardOutput());
}
void MainWindow::showFileContextMenu(const QPoint &pos) {
    QModelIndex index = fileTree->indexAt(pos);
    if (!index.isValid()) return;

    QString path = fsModel->filePath(index);
    QMenu menu;
    if (QFileInfo(path).isDir()) {
        menu.addAction("New File", this, &MainWindow::newFile);
        menu.addAction("New Folder", this, &MainWindow::newFolder);
    } else {
        menu.addAction("Open", this, [=]() { onFileDoubleClicked(index); });
        menu.addAction("Delete", this, &MainWindow::deleteFile);
        menu.addAction("Rename", this, &MainWindow::renameFile);
        menu.addAction("Format", this, [=]() { formatFile(path); });
        menu.addAction("Git Diff", this, &MainWindow::showGitDiff);
    }
    menu.exec(fileTree->viewport()->mapToGlobal(pos));
}
QLineSeries *series = new QLineSeries();
series->append(1, 5); // Day 1: 5 commits
series->append(2, 8); // Day 2: 8 commits

QChart *chart = new QChart();
chart->addSeries(series);
chart->setTitle("Commits per Day");
QChartView *chartView = new QChartView(chart);
chartView->setRenderHint(QPainter::Antialiasing);
QDockWidget *chartDock = new QDockWidget("Commit History", this);
chartDock->setWidget(chartView);
addDockWidget(Qt::BottomDockWidgetArea, chartDock);
void MainWindow::setupCommitChart() {
    QLineSeries *series = new QLineSeries();
    series->append(1, 5); // Day 1: 5 commits
    series->append(2, 8); // Day 2: 8 commits
    series->append(3, 6); // Day 3: 6 commits
    series->append(4, 10); // Day 4: 10 commits
    series->append(5, 7); // Day 5: 7 commits

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Commits per Day");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Day");
    chart->axes(Qt::Vertical).first()->setTitleText("Number of Commits");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDockWidget *chartDock = new QDockWidget("Commit History", this);
    chartDock->setWidget(chartView);
    addDockWidget(Qt::BottomDockWidgetArea, chartDock);
}
void MainWindow::setupPropertyEditor() {
    if (!selectedWidget) return;

    QDockWidget *propertyDock = new QDockWidget("Properties", this);
    QWidget *propertyWidget = new QWidget;
    propertyDock->setWidget(propertyWidget);
    addDockWidget(Qt::RightDockWidgetArea, propertyDock);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Properties"));
    propertyWidget->setLayout(layout);
}
void MainWindow::reviewCode(QString filePath) {
    QFile f(filePath);
    f.open(QIODevice::ReadOnly);
    QString code = f.readAll();

    QJsonObject req;
    req["code"] = code;

    QNetworkReply *r = manager->post(
        QNetworkRequest(QUrl("https://shuriken-ai.example.com/review")),
        QJsonDocument(req).toJson()
    );

    connect(r, &QNetworkReply::finished, this, [=]() {
        QString report = QString::fromUtf8(r->readAll());
        ui->logOutput->append("AI Review Report:\n" + report);
    });
}
void MainWindow::commitChanges() {
    bool ok;
    QString msg = QInputDialog::getText(this, "Commit Changes", "Enter commit message:", QLineEdit::Normal, "", &ok);
    if (ok && !msg.isEmpty()) {
        // Proceed with the commit using the provided message
    }
}
    QProcess git;
    git.start("git", {"log", "--grep", "fix", "--pretty=oneline"});
    git.waitForFinished();
    ui->logOutput->append("Bugs Fixed:\n" + git.readAllStandardOutput());
}
void MainWindow::showFileContextMenu(const QPoint &pos) {
    QModelIndex index = fileTree->indexAt(pos);
    if (!index.isValid()) return;

    QString path = fsModel->filePath(index);
    QMenu menu;
    if (QFileInfo(path).isDir()) {
        menu.addAction("New File", this, &MainWindow::newFile);
        menu.addAction("New Folder", this, &MainWindow::newFolder);
    } else {
        menu.addAction("Open", this, [=]() { onFileDoubleClicked(index); });
        menu.addAction("Delete", this, &MainWindow::deleteFile);
        menu.addAction("Rename", this, &MainWindow::renameFile);
        menu.addAction("Format", this, [=]() { formatFile(path); });
        menu.addAction("Git Diff", this, &MainWindow::showGitDiff);
    }
    menu.exec(fileTree->viewport()->mapToGlobal(pos));
}
QLineSeries *series = new QLineSeries();
series->append(1, 5); // Day 1: 5 commits
series->append(2, 8); // Day 2: 8 commits

QChart *chart = new QChart();
chart->addSeries(series);
chart->setTitle("Commits per Day");
QChartView *chartView = new QChartView(chart);
chartView->setRenderHint(QPainter::Antialiasing);
QDockWidget *chartDock = new QDockWidget("Commit History", this);
chartDock->setWidget(chartView);
addDockWidget(Qt::BottomDockWidgetArea, chartDock);
void MainWindow::setupCommitChart() {
    QLineSeries *series = new QLineSeries();
    series->append(1, 5); // Day 1: 5 commits
    series->append(2, 8); // Day 2: 8 commits
    series->append(3, 6); // Day 3: 6 commits
    series->append(4, 10); // Day 4: 10 commits
    series->append(5, 7); // Day 5: 7 commits

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Commits per Day");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setTitleText("Day");
    chart->axes(Qt::Vertical).first()->setTitleText("Number of Commits");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDockWidget *chartDock = new QDockWidget("Commit History", this);
    chartDock->setWidget(chartView);
    addDockWidget(Qt::BottomDockWidgetArea, chartDock);
}
void MainWindow::setupPropertyEditor() {
    if (!selectedWidget) return;

    QDockWidget *propertyDock = new QDockWidget("Properties", this);
    QWidget *propertyWidget = new QWidget;
    propertyDock->setWidget(propertyWidget);
    addDockWidget(Qt::RightDockWidgetArea, propertyDock);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Properties"));
    propertyWidget->setLayout(layout);
}
void MainWindow::reviewCode(QString filePath) {
    QFile f(filePath);
    f.open(QIODevice::ReadOnly);
    QString code = f.readAll();

    QJsonObject req;
    req["code"] = code;

    QNetworkReply *r = manager->post(
        QNetworkRequest(QUrl("https://shuriken-ai.example.com/review")),
        QJsonDocument(req).toJson()
    );

    connect(r, &QNetworkReply::finished, this, [=]() {
        QString report = QString::fromUtf8(r->readAll());
        ui->logOutput->append("AI Review Report:\n" + report);
    });
}
void MainWindow::commitChanges() {
    bool ok;
    QString msg = QInputDialog::getText(this, "Commit Changes", "Enter commit message:", QLineEdit::Normal, "", &ok);
    if (ok && !msg.isEmpty()) {
        QProcess gitAdd;
        gitAdd.start("git", {"add", "."});
        gitAdd.waitForFinished();

        QProcess gitCommit;
        gitCommit.start("git", {"commit", "-m", msg});
        gitCommit.waitForFinished();

        ui->logOutput->append("Committed changes with message: " + msg);
    }
}
void MainWindow::packageInstaller() {
    if (builtExe.isEmpty() || !QFile::exists(builtExe)) {
        QMessageBox::warning(this, "Error", "Built executable not found. Please build the project first.");
        return;
    }
    if (projectRoot.isEmpty() || !QDir(projectRoot).exists()) {
        QMessageBox::warning(this, "Error", "Project root directory is invalid.");
        return;
    }
    if (ui->installerTargetCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a target platform for the installer.");
        return;
    }

    QString target = ui->installerTargetCombo->currentText();
    QString installerScript = projectRoot + "/installers/" + target + ".sh";

    QFile scriptFile(installerScript);
    if (!scriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to create installer script.");
        return;
    }

    QTextStream out(&scriptFile);
    out << "#!/bin/bash\n";
    out << "echo 'Installing Shuriken on " << target << "...'\n";
    out << "cp " << builtExe << " /usr/local/bin/shuriken\n";
    out << "echo 'Installation complete.'\n";

    scriptFile.close();
    ui->logOutput->append("Installer script created: " + installerScript);
}
NotificationSocket *notifSocket = new NotificationSocket(globalUserToken, this);

connect(notifSocket, &NotificationSocket::newNotification, this, [=](QString msg) {
    // Show popup message
    QMessageBox::information(this, "🔔 New Notification", msg);
    // Also add to Notifications panel
    notificationsList->addItem("🔔 " + msg);
});
void MainWindow::packageInstaller() {
    if (builtExe.isEmpty() || !QFile::exists(builtExe)) {
        QMessageBox::warning(this, "Error", "Built executable not found. Please build the project first.");
        return;
    }
    if (projectRoot.isEmpty() || !QDir(projectRoot).exists()) {
        QMessageBox::warning(this, "Error", "Project root directory is invalid.");
        return;
    }
    if (ui->installerTargetCombo->currentText().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please select a target platform for the installer.");
        return;
    }

    QString target = ui->installerTargetCombo->currentText();
    QString appName = QFileInfo(builtExe).baseName();
    QString version = "1.0.0"; // This could be fetched from a config file or input field
    QString outInstaller = projectRoot + "/" + appName + "_" + version + "_" + target.replace(" ", "_") + ".exe";

    ui->installerLogOutput->clear();
    ui->progressBar->setValue(0);
    ui->statusBar->showMessage("Packaging installer...");

    if (target == "Windows NSIS") {
        QString nsisScript = projectRoot + "/installers/windows_installer.nsi";
        QFile scriptFile(nsisScript);
        if (!scriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Failed to create NSIS script.");
            return;
        }

        QTextStream out(&scriptFile);
        out << "OutFile \"" << outInstaller << "\"\n";
        out << "InstallDir \"$PROGRAMFILES\\" << appName << "\"\n";
        out << "Page directory\n";
        out << "Page instfiles\n";
        out << "Section \"Install\"\n";
        out << "SetOutPath \"$INSTDIR\"\n";
        out << "File \"" << builtExe << "\"\n";
        out << "CreateShortCut \"$DESKTOP\\" << appName << ".lnk\" \"$INSTDIR\\" << QFileInfo(builtExe).fileName() << "\"\n";
        out << "SectionEnd\n";
        scriptFile.close();

        QProcess *makensis = new QProcess(this);
        makensis->setProgram("makensis");
        makensis->setArguments({nsisScript});
        makensis->start();