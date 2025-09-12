#include "TerminalWidget.h"
#include <QVBoxLayout>
#include <QScrollBar>

TerminalWidget::TerminalWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    output = new QPlainTextEdit;
    output->setReadOnly(true);
    output->setFont(QFont("Consolas", 10));

    input = new QLineEdit;
    input->setPlaceholderText("Enter command...");

    layout->addWidget(output);
    layout->addWidget(input);
    setLayout(layout);

    process = new QProcess(this);

#if defined(Q_OS_WIN)
    process->start("powershell.exe");
#elif defined(Q_OS_MAC)
    process->start("/bin/zsh");
#else
    process->start("/bin/bash");
#endif

    connect(process, &QProcess::readyReadStandardOutput, this, &TerminalWidget::onReadyRead);
    connect(process, &QProcess::readyReadStandardError, this, &TerminalWidget::onReadyRead);
    connect(input, &QLineEdit::returnPressed, this, &TerminalWidget::onCommandEntered);
}

void TerminalWidget::onReadyRead() {
    QString text = process->readAllStandardOutput();
    text += process->readAllStandardError();
    output->appendPlainText(text.trimmed());
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

void TerminalWidget::onCommandEntered() {
    QString cmd = input->text().trimmed();
    if (!cmd.isEmpty()) {
        process->write((cmd + "\n").toUtf8());
        input->clear();
    }
}
