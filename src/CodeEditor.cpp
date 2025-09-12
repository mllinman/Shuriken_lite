#include "CodeEditor.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QTextCharFormat>
#include <QFont>
#include <QMouseEvent>
#include <QTextBlock>
#include <QPainter>
#include "CodeCompleter.h"
#include <QCompleter>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <clang-c/Index.h>

CppHighlighter::CppHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::cyan);
    QStringList keywords = {"class", "const", "void", "int", "float", "double", "if", "else", "return", "for", "while"};
    for (const QString &kw : keywords) {
        setFormatForPattern("\\b" + kw + "\\b", keywordFormat);
    }
}

void CppHighlighter::highlightBlock(const QString &text) {
    QTextCharFormat kwFormat;
    kwFormat.setForeground(Qt::cyan);
    QStringList keywords = {"class", "namespace", "if", "else", "return", "for", "while", "switch", "case", "break"};
    for (const QString &kw : keywords) {
        QRegularExpression re("\\b" + kw + "\\b");
        auto it = re.globalMatch(text);
        while (it.hasNext()) {
            auto match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), kwFormat);
        }
    }

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::green);
    QRegularExpression comment("//[^\n]*");
    auto it2 = comment.globalMatch(text);
    while (it2.hasNext()) {
        auto match = it2.next();
        setFormat(match.capturedStart(), match.capturedLength(), commentFormat);
    }
}

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    setFont(QFont("Consolas", 11));
    new CppHighlighter(document());
}

void CodeEditor::loadFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    setPlainText(in.readAll());
    currentFile = filePath;
}

void CodeEditor::saveFile(const QString &filePath) {
    QFile file(filePath.isEmpty() ? currentFile : filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << toPlainText();
}
void CodeEditor::mousePressEvent(QMouseEvent *event) {
    if (event->x() < 30) { // margin area
        QTextCursor cursor = cursorForPosition(event->pos());
        int line = cursor.blockNumber() + 1;
        toggleBreakpoint(line);
        return;
    }
    QPlainTextEdit::mousePressEvent(event);
}

void CodeEditor::toggleBreakpoint(int line) {
    QString key = filePath + ":" + QString::number(line);
    if (breakpoints.contains(key)) {
        breakpoints.remove(key);
    } else {
        breakpoints.insert(key);
    }
    viewport()->update(); // redraw with markers
}

void CodeEditor::paintEvent(QPaintEvent *e) {
    QPlainTextEdit::paintEvent(e);
    QPainter painter(viewport());
    for (const QString &bp : breakpoints) {
        QStringList parts = bp.split(":");
        if (parts.size() != 2 || parts[0] != filePath) continue;
        int line = parts[1].toInt();

        QTextBlock block = document()->findBlockByNumber(line - 1);
        QRect rect = blockBoundingGeometry(block).translated(contentOffset()).toRect();

        painter.setBrush(Qt::red);
        painter.drawEllipse(5, rect.top() + 5, 10, 10);
    }
}
CodeCompleter *globalCompleter = new CodeCompleter;

void CodeEditor::keyPressEvent(QKeyEvent *event) {
    QPlainTextEdit::keyPressEvent(event);

    if (event->text().isEmpty()) return;

    QTextCursor cursor = textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.positionInBlock() + 1;

    QStringList suggestions = globalCompleter->completeAt(currentFile, line, col);
    if (suggestions.isEmpty()) return;

    QCompleter *comp = new QCompleter(suggestions, this);
    comp->setWidget(this);
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->complete(cursorRect());
}
void CodeEditor::setFormatForPattern(const QString &pattern, const QTextCharFormat &format) {
    QRegularExpression re(pattern);
    auto it = re.globalMatch(toPlainText());
    while (it.hasNext()) {
        auto match = it.next();
        setFormat(match.capturedStart(), match.capturedLength(), format);
    }
}
void CodeEditor::extractFunction(QString funcName) {
    QTextCursor sel = textCursor();
    QString code = sel.selectedText();

    if (code.isEmpty()) return;

    // Insert new function at bottom
    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::End);
    cur.insertText("\n\nvoid " + funcName + "() {\n" + code + "\n}\n");

    // Replace selection with call
    sel.insertText(funcName + "();");
}
void CodeEditor::renameSymbol(QString newName) {
    QTextCursor cursor = textCursor();
    QString selected = cursor.selectedText();
    if (selected.isEmpty()) return;

    QString content = toPlainText();
    content.replace(QRegExp("\\b" + QRegularExpression::escape(selected) + "\\b"), newName);
    setPlainText(content);
}
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int)blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}
int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    return 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
}
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
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
void CodeEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
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
    struct CursorUpdate {
    QString user;
    int line;
    int column;
};

void CodeEditor::broadcastCursor() {
    QTextCursor cur = textCursor();
    int line = cur.blockNumber() + 1;
    int col = cur.positionInBlock() + 1;

    QJsonObject obj;
    obj["user"] = currentUser;
    obj["line"] = line;
    obj["col"] = col;

    socket->sendTextMessage(QJsonDocument(obj).toJson());
}

void CodeEditor::applyRemoteCursor(QJsonObject obj) {
    QString user = obj["user"].toString();
    int line = obj["line"].toInt();
    int col = obj["col"].toInt();

    QTextCursor cursor(document()->findBlockByNumber(line-1));
    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, col);
    
    QTextEdit::ExtraSelection sel;
    sel.cursor = cursor;
    sel.format.setBackground(QColor(100,200,255,100));
    sel.format.setProperty(QTextFormat::UserProperty, user);

    QList<QTextEdit::ExtraSelection> sels = extraSelections();
    sels.append(sel);
    setExtraSelections(sels);
}
    connect(socket, &QWebSocket::textMessageReceived, this, [=](const QString &msg) {
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        if (!doc.isObject()) return;
        applyRemoteCursor(doc.object());
    });
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::broadcastCursor);
    connect(socket, &QWebSocket::disconnected, this, [=]() {
        ui->statusBar->showMessage("Disconnected from collaboration server");
    });
    void CodeEditor::aiSuggest() {
    QString code = toPlainText();
    int pos = textCursor().position();

    QNetworkRequest req(QUrl("https://shuriken-ai.example.com/complete"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload;
    payload["code"] = code;
    payload["cursor"] = pos;

    QNetworkReply *reply = manager->post(req, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QString suggestion = QString::fromUtf8(reply->readAll());
        insertPlainText(suggestion);
    });
}
    connect(aiButton, &QPushButton::clicked, this, &CodeEditor::aiSuggest);
}
void CodeEditor::fixErrorWithAI(QString errorMsg, QString codeContext) {
    QJsonObject req;
    req["error"] = errorMsg;
    req["context"] = codeContext;

    QNetworkReply *r = manager->post(QNetworkRequest(QUrl("https://shuriken-ai.example.com/fix")),
                                     QJsonDocument(req).toJson());
    connect(r, &QNetworkReply::finished, this, [=]() {
        QString fix = QString::fromUtf8(r->readAll());
        replaceSelectedText(fix);
    });
}
void CodeEditor::replaceSelectedText(const QString &newText) {
    QTextCursor cursor = textCursor();
    if (cursor.hasSelection()) {
        cursor.insertText(newText);
    }
}
void CodeEditor::updateDiagnostics() {
    CXTranslationUnit unit = clang_parseTranslationUnit(globalCompleter->index,
        currentFile.toUtf8().data(), nullptr, 0, nullptr, 0, CXTranslationUnit_DetailedPreprocessingRecord);

    unsigned numDiags = clang_getNumDiagnostics(unit);
    QList<QTextEdit::ExtraSelection> squiggles;

    for (unsigned i = 0; i < numDiags; i++) {
        CXDiagnostic diag = clang_getDiagnostic(unit, i);
        CXSourceLocation loc = clang_getDiagnosticLocation(diag);

        CXFile file;
        unsigned line, col;
        clang_getSpellingLocation(loc, &file, &line, &col, nullptr);

        QTextCursor cursor(document()->findBlockByNumber(line - 1));
        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        if (clang_getDiagnosticSeverity(diag) == CXDiagnostic_Error)
            sel.format.setUnderlineColor(Qt::red), sel.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        else
            sel.format.setUnderlineColor(Qt::yellow), sel.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);

        squiggles << sel;
        clang_disposeDiagnostic(diag);
    }
    setExtraSelections(squiggles);
    clang_disposeTranslationUnit(unit);
}
void GuiDesigner::updateProperty(const QString &name, const QString &value) {
    QTreeWidgetItem *item = findPropertyItem(name);
    if (item) {
        item->setText(1, value);
    } else {
        QTreeWidgetItem *newItem = new QTreeWidgetItem(properties);
        newItem->setText(0, name);
        newItem->setText(1, value);
        properties->addTopLevelItem(newItem);
    }
}
QTreeWidgetItem* GuiDesigner::findPropertyItem(const QString &name) {
    for (int i = 0; i < properties->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = properties->topLevelItem(i);
        if (item->text(0) == name) return item;
    }
    return nullptr;
}
void GuiDesigner::refreshProperties() {
    properties->clear();
    // Repopulate the properties tree (this is just an example)
    updateProperty("text", "Button");
}