class Debugger : public QObject {
    Q_OBJECT
public:
    Debugger(QString exePath, QObject *parent = nullptr) : QObject(parent), exePath(exePath) {
        process = new QProcess(this);
    }

    void start() {
        process->start("gdb", {"--interpreter=mi", exePath});
    }

    void sendCommand(QString cmd) {
        process->write((cmd + "\n").toUtf8());
    }

signals:
    void debuggerOutput(QString);

private:
    QString exePath;
    QProcess *process;
};
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
void Debugger::broadcastState(QString state) {
    QJsonObject obj;
    obj["type"] = "debug-state";
    obj["data"] = state;
    socket->sendTextMessage(QJsonDocument(obj).toJson());
}

void Debugger::applyRemoteState(QJsonObject obj) {
    QString state = obj["data"].toString();
    ui->logOutput->append("[Remote Debug] " + state);
}
    connect(socket, &QWebSocket::textMessageReceived, this, [=](const QString &msg) {
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        if (!doc.isObject()) return;
        applyRemoteState(doc.object());
    });
    connect(this, &Debugger::debuggerOutput, this, &Debugger::broadcastState);
    connect(socket, &QWebSocket::disconnected, this, [=]() {
        ui->statusBar->showMessage("Disconnected from collaboration server");
    });
}

