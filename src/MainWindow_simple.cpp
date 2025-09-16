#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Connect the buttons that actually exist in the UI
    connect(ui->browseButton, &QPushButton::clicked, this, &MainWindow::browseSource);
    connect(ui->buildButton, &QPushButton::clicked, this, &MainWindow::buildApp);
    connect(ui->packageButton, &QPushButton::clicked, this, &MainWindow::packageInstaller);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::browseSource()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Select Source Folder");
    if (!folder.isEmpty()) {
        ui->repoInput->setText(folder);
        ui->logOutput->append("Source folder selected: " + folder);
    }
}

void MainWindow::buildApp()
{
    QString sourceFolder = ui->repoInput->text();
    if (sourceFolder.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a source folder first.");
        return;
    }
    
    ui->logOutput->append("Starting build process for: " + sourceFolder);
    ui->logOutput->append("Build functionality will be implemented here.");
    
    // TODO: Implement actual build logic
    QMessageBox::information(this, "Build", "Build functionality is not yet implemented.");
}

void MainWindow::packageInstaller()
{
    ui->logOutput->append("Starting installer packaging...");
    ui->logOutput->append("Package installer functionality will be implemented here.");
    
    // TODO: Implement actual packaging logic
    QMessageBox::information(this, "Package", "Package installer functionality is not yet implemented.");
}