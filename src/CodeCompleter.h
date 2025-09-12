#pragma once
#include <QObject>
#include <QStringList>
#include <clang-c/Index.h>

class CodeCompleter : public QObject {
    Q_OBJECT
public:
    CodeCompleter(QObject *parent = nullptr);
    QStringList completeAt(const QString &file, int line, int column);

private:
    CXIndex index;
};
