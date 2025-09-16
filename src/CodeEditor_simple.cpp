#include "CodeEditor.h"
#include <QTextCharFormat>
#include <QTextDocument>
#include <QApplication>
#include <QPainter>
#include <QTextBlock>

// CppHighlighter implementation
CppHighlighter::CppHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
}

void CppHighlighter::highlightBlock(const QString &text) {
    // Simple C++ syntax highlighting
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor("#0000FF"));
    keywordFormat.setFontWeight(QFont::Bold);
    
    QStringList keywordPatterns = {
        "\\bclass\\b", "\\bif\\b", "\\belse\\b", "\\bfor\\b", "\\bwhile\\b",
        "\\breturn\\b", "\\bvoid\\b", "\\bint\\b", "\\bstring\\b", "\\bbool\\b"
    };
    
    for (const QString &pattern : keywordPatterns) {
        QRegularExpression expression(pattern);
        QRegularExpressionMatchIterator i = expression.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            setFormat(match.capturedStart(), match.capturedLength(), keywordFormat);
        }
    }
}

void CppHighlighter::setFormatForPattern(const QString &pattern, const QTextCharFormat &format) {
    // Stub implementation
}

// LineNumberArea helper class
class LineNumberArea : public QWidget {
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}
    
    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

// CodeEditor implementation
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    lineNumberArea = new LineNumberArea(this);
    highlighter = new CppHighlighter(document());
    
    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, [this](const QRect &rect, int dy) {
        if (dy)
            lineNumberArea->scroll(0, dy);
        else
            lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    });
    
    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CodeEditor::loadFile(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setPlainText(file.readAll());
        this->filePath = filePath;
        currentFile = filePath;
    }
}

void CodeEditor::saveFile(const QString &filePath) {
    QFile file(filePath.isEmpty() ? this->filePath : filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << toPlainText();
        if (!filePath.isEmpty()) {
            this->filePath = filePath;
            currentFile = filePath;
        }
    }
}

void CodeEditor::toggleBreakpoint(int line) {
    QString lineStr = QString::number(line);
    if (breakpoints.contains(lineStr)) {
        breakpoints.remove(lineStr);
    } else {
        breakpoints.insert(lineStr);
    }
    update();
}

void CodeEditor::mousePressEvent(QMouseEvent *event) {
    if (event->position().x() < 30) { // margin area
        QTextCursor cursor = cursorForPosition(event->pos());
        int line = cursor.blockNumber() + 1;
        toggleBreakpoint(line);
    }
    QPlainTextEdit::mousePressEvent(event);
}

void CodeEditor::setFormatForPattern(const QString &pattern, const QTextCharFormat &format) {
    if (highlighter) {
        highlighter->setFormatForPattern(pattern, format);
    }
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);
    
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                           Qt::AlignRight, number);
        }
        
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;
    
    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    
    setExtraSelections(extraSelections);
}