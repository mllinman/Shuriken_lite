#include "IssueCommentsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

IssueCommentsDialog::IssueCommentsDialog(int issueId, IssueManager *im, QWidget *parent)
    : QDialog(parent), issueId(issueId), im(im) {

    setWindowTitle("Issue Discussion");
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    commentsList = new QListWidget;
    commentInput = new QLineEdit;
    commentInput->setPlaceholderText("Write a comment...");
    sendBtn = new QPushButton("Send");

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(commentInput);
    inputLayout->addWidget(sendBtn);

    layout->addWidget(commentsList);
    layout->addLayout(inputLayout);

    connect(sendBtn, &QPushButton::clicked, this, &IssueCommentsDialog::postComment);
    connect(im, &IssueManager::commentsListed, this, [=](QStringList list) {
        commentsList->clear();
        commentsList->addItems(list);
    });
    connect(im, &IssueManager::commentAdded, this, [=](QString body) {
        commentsList->addItem("💬 You: " + body);
        commentInput->clear();
    });

    im->listComments(issueId);
}

void IssueCommentsDialog::postComment() {
    QString text = commentInput->text().trimmed();
    if (!text.isEmpty()) {
        im->addComment(issueId, text);
    }
}
connect(issuesList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item) {
    int issueId = item->data(Qt::UserRole).toInt(); // make sure you stored it when populating list
    IssueCommentsDialog dlg(issueId, im, this);
    dlg.exec();
});
void CodeEditor::mousePressEvent(QMouseEvent *event) {
    if (event->x() < 30) { // margin area
        QTextCursor cursor = cursorForPosition(event->pos());
        int line = cursor.blockNumber() + 1;
        toggleBreakpoint(line);
        return;
    }
    QPlainTextEdit::mousePressEvent(event);
}
