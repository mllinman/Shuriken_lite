#pragma once
#include <QDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include "IssueManager.h"

class IssueCommentsDialog : public QDialog {
    Q_OBJECT
public:
    IssueCommentsDialog(int issueId, IssueManager *im, QWidget *parent = nullptr);

private slots:
    void postComment();

private:
    int issueId;
    IssueManager *im;
    QListWidget *commentsList;
    QLineEdit *commentInput;
    QPushButton *sendBtn;
};