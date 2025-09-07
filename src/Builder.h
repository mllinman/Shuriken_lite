#pragma once
#include <QString>
#include <QStringList>

class Builder {
public:
    Builder(QString llvmPath);
    bool compileProject(QString sourceDir, QString outputExe, QString &log);

private:
    QString llvmPath;
    bool runClang(QStringList args, QString &log);
};
// Pseudo-code of build process inside GUI
void Builder::buildProject(QString sourceDir) {
    auto files = findCppFiles(sourceDir);

    // Compile each source file
    for (auto &file : files) {
        runEmbeddedCompiler({"-c", file, "-o", file + ".o"});
    }

    // Link object files into final exe
    runEmbeddedCompiler({"file1.o", "file2.o", "-o", "App.exe"});

    // Package exe into installer
    createSelfExtractingInstaller("App.exe", "AppInstaller.exe");
}


