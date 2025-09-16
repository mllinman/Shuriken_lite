#include "Debugger.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebSocket>

Debugger::Debugger(QString exePath, QObject *parent) : QObject(parent), exePath(exePath) {
    process = new QProcess(this);
    
    connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
        QString output = process->readAllStandardOutput();
        emit debuggerOutput(output);
    });
    connect(process, &QProcess::readyReadStandardError, this, [=]() {
        QString error = process->readAllStandardError();
        emit debuggerOutput(error);
    });
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [=](int exitCode, QProcess::ExitStatus status) {
        emit debuggerOutput(QString("Debugger exited with code %1").arg(exitCode));
    });
}

void Debugger::start() {
    process->start("gdb", {"--interpreter=mi", exePath});
}

void Debugger::sendCommand(QString cmd) {
    process->write((cmd + "\n").toUtf8());
}

