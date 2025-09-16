#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QObject>
#include <QProcess>
#include <QString>

class Debugger : public QObject {
    Q_OBJECT
public:
    Debugger(QString exePath, QObject *parent = nullptr);
    void start();
    void sendCommand(QString cmd);

signals:
    void debuggerOutput(QString);

private:
    QString exePath;
    QProcess *process;
};

#endif // DEBUGGER_H