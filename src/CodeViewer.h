#ifndef CODEVIEWER_H
#define CODEVIEWER_H

#include <QPlainTextEdit>
#include <QFont>

class CodeViewer : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeViewer(QWidget *parent = nullptr);
    void loadFile(const QString &filePath, int highlightLine = -1);
};

#endif // CODEVIEWER_H