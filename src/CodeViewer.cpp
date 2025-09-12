#include <QPlainTextEdit>
#include <QFile>
#include <QTextStream>
#include "CodeViewer.h"


class CodeViewer : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeViewer(QWidget *parent = nullptr) : QPlainTextEdit(parent) {
        setReadOnly(true);
        setFont(QFont("Consolas", 10));
    }

    void loadFile(const QString &filePath, int highlightLine = -1) {
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
};
