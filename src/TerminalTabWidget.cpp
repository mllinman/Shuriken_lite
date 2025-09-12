#include "TerminalTabWidget.h"
#include <QPushButton>
#include <QTabBar>

TerminalTabWidget::TerminalTabWidget(QWidget *parent) : QTabWidget(parent) {
    setTabsClosable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, [=](int index) {
        QWidget *w = widget(index);
        removeTab(index);
        w->deleteLater();
    });

    newTerminal(); // start with one
}

void TerminalTabWidget::newTerminal() {
    TerminalWidget *term = new TerminalWidget;
    int idx = addTab(term, "Terminal " + QString::number(count() + 1));
    setCurrentIndex(idx);
}
void TerminalWidget::onReadyRead() {
    QString raw = process->readAllStandardOutput();
    raw += process->readAllStandardError();

    // Basic ANSI color parsing
    QString text = raw;
    text.replace(QRegExp("\\x1B\\[[0-9;]*m"), ""); // strip codes for now

    // Future: parse \033[31m = red, \033[33m = yellow, etc.
    output->appendHtml("<pre>" + text.toHtmlEscaped() + "</pre>");
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}
void TerminalWidget::onCommandEntered() {
    QString cmd = input->text().trimmed();
    if (!cmd.isEmpty()) {
        process->write((cmd + "\n").toUtf8());
        input->clear();
    }
}
