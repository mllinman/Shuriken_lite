#ifndef PROJECTHUBPANEL_H
#define PROJECTHUBPANEL_H

#include <QWidget>

class QTimer;
class QListWidget;
class QPushButton;

class ProjectHubPanel : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProjectHubPanel(QWidget *parent = nullptr);

private:
    QTimer *feedRefreshTimer;
    QListWidget *feedList;
    QPushButton *refreshFeedBtn;
    QTimer *issuesRefreshTimer;
    QListWidget *issuesList;
    QPushButton *refreshIssuesBtn;
    QPushButton *newIssueBtn;
    int currentProjectId;
};

#endif // PROJECTHUBPANEL_H