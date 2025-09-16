
#pragma once
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QSet>
#include <QTextDocument>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QRegularExpression>
#include <QFile>
#include <QTextStream>

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
    int lineNumberAreaWidth();
    void lineNumberAreaPaintEvent(QPaintEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();

private:
    QString currentFile;
    QString filePath;
    QSet<QString> breakpoints;
    CppHighlighter *highlighter;
    QWidget *lineNumberArea;
    void setFormatForPattern(const QString &pattern, const QTextCharFormat &format);
};
