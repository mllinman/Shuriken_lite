#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Builder.h"
#include "Installer.h"
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseSource);
    connect(ui->buildButton, &QPushButton::clicked, this, &MainWindow::buildApp);
    connect(ui->packageButton, &QPushButton::clicked, this, &MainWindow::packageInstaller);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::browseSource() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Source Folder");
    if (!dir.isEmpty()) {
        ui->repoInput->setText(dir);
    }
}

void MainWindow::buildApp() {
    QString sourceDir = ui->repoInput->text();
    if (sourceDir.isEmpty()) {
        ui->logOutput->append("Please select a source folder first.");
        return;
    }

    QString log;
    Builder builder("llvm/prebuilt_clang/");
    QString exeOut = QDir(sourceDir).filePath("OutputApp.exe");

    if (builder.compileProject(sourceDir, exeOut, log)) {
        ui->logOutput->append("Build successful: " + exeOut);
        builtExe = exeOut;
    } else {
        ui->logOutput->append("Build failed.");
    }
    ui->logOutput->append(log);
}

void MainWindow::packageInstaller() {
    if (builtExe.isEmpty()) {
        ui->logOutput->append("No built executable to package.");
        return;
    }

    QString outInstaller = QFileDialog::getSaveFileName(this, "Save Installer", "ShurikenInstaller.exe");
    if (outInstaller.isEmpty()) return;

    QString log;
    Installer installer("MyApp", builtExe);
    if (installer.createInstaller(outInstaller, log)) {
        ui->logOutput->append("Installer created: " + outInstaller);
    } else {
        ui->logOutput->append("Installer packaging failed.");
    }
    ui->logOutput->append(log);
    QMessageBox::information(this, "Packaging Complete", "Installer packaging complete.");
}

