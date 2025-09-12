
#pragma once
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QSet>
#include <QTextDocument>
#include <QMouseEvent>
#include <QPaintEvent>


class CppHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    CppHighlighter(QTextDocument *parent = nullptr);
    void setFormatForPattern(const QString &pattern, const QTextCharFormat &format);
protected:
    void highlightBlock(const QString &text) override;
};

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);
    void loadFile(const QString &filePath);
    void saveFile(const QString &filePath);
    void toggleBreakpoint(int line);
    bool hasBreakpoint(int line) const {
        return breakpoints.contains(QString::number(line));
    }

private:
    QString currentFile;
    QString filePath;
    QSet<QString> breakpoints;
    CppHighlighter *highlighter;
    void setFormatForPattern(const QString &pattern, const QTextCharFormat &format);
    void mousePressEvent(QMouseEvent *event) override;
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void resizeEvent(QResizeEvent *event) override;
    QWidget *lineNumberArea;
};
