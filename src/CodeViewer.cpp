#include "CodeViewer.h"
#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>

CodeViewer::CodeViewer(QWidget *parent) : QPlainTextEdit(parent) {
    setReadOnly(true);
    setFont(QFont("Consolas", 10));
}

void CodeViewer::loadFile(const QString &filePath, int highlightLine) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    setPlainText(in.readAll());

    if (highlightLine > 0) {
        QTextCursor cursor(document()->findBlockByLineNumber(highlightLine - 1));
        setTextCursor(cursor);

        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format.setBackground(Qt::darkRed);
        sel.format.setForeground(Qt::white);

        QList<QTextEdit::ExtraSelection> selections;
        selections << sel;
        setExtraSelections(selections);
    }
}
