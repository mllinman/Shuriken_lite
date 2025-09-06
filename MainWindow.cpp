#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), process(new QProcess(this)) {
    ui->setupUi(this);

    connect(ui->buildButton, &QPushButton::clicked, this, &MainWindow::onCloneAndBuild);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::onProcessOutput);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onCloneAndBuild() {
    QString repoPath = ui->repoInput->text();

    if (repoPath.startsWith("http")) {
        ui->logOutput->append("Cloning GitHub repo...");
        process->start("git", {"clone", repoPath, "temp_project"});
        process->waitForFinished();
        repoPath = "temp_project";
    }

    ui->logOutput->append("Running CMake build...");
    QDir buildDir(repoPath + "/build");
    if (!buildDir.exists()) QDir().mkdir(repoPath + "/build");

    process->setWorkingDirectory(buildDir.absolutePath());
    process->start("cmake", {".."}); // configure
    process->waitForFinished();

    process->start("cmake", {"--build", "."}); // build
}

void MainWindow::onProcessOutput() {
    ui->logOutput->append(process->readAllStandardOutput());
}
