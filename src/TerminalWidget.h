#pragma once
#include <QWidget>
#include <QProcess>
#include <QPlainTextEdit>
#include <QLineEdit>

class TerminalWidget : public QWidget {
    Q_OBJECT
public:
    explicit TerminalWidget(QWidget *parent = nullptr);

private slots:
    void onReadyRead();
    void onCommandEntered();

private:
    QProcess *process;
    QPlainTextEdit *output;
    QLineEdit *input;
};
