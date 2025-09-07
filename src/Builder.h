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


