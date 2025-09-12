#include "Builder.h"
#include <QDir>
#include <QFileInfoList>
#include <QProcess>
#include <QTextStream>
#include <QDirIterator>
#include <QFileInfo>

Builder::Builder(QString llvmPath) : llvmPath(llvmPath) {}

bool Builder::compileProject(QString sourceDir, QString outputExe, QString &log) {
    QDir dir(sourceDir);
    if (!dir.exists()) {
        log.append("Source directory not found.\n");
        return false;
    }

    // Collect source files
    QStringList cppFiles;
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.cpp", QDir::Files, QDir::Name);
    for (const QFileInfo &file : files) {
        cppFiles << file.absoluteFilePath();
    }

    if (cppFiles.isEmpty()) {
        log.append("No .cpp files found.\n");
        return false;
    }

    log.append("Compiling project...\n");

    // Compile and link
    QStringList args;
    args << "-std=c++20" << "-O2";
    args << cppFiles;
    args << "-o" << outputExe;

    return runClang(args, log);
}

bool Builder::runClang(QStringList args, QString &log) {
    QProcess process;
    QString clangBin = QDir(llvmPath).filePath("clang.exe");

    process.start(clangBin, args);
    if (!process.waitForStarted()) {
        log.append("Failed to start clang.\n");
        return false;
    }

    process.waitForFinished(-1);
    QString output = process.readAllStandardOutput();
    QString errors = process.readAllStandardError();

    if (!output.isEmpty()) log.append(output + "\n");
    if (!errors.isEmpty()) log.append(errors + "\n");

    return process.exitCode() == 0;
}

