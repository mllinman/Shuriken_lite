#include <QTimer>

class ProjectHubPanel
{
private:
    QTimer *feedRefreshTimer;
    QListWidget *feedList;
    QPushButton *refreshFeedBtn;
    QTimer *issuesRefreshTimer;
    QListWidget *issuesList;
    QPushButton *refreshIssuesBtn;
    QPushButton *newIssueBtn;
    int currentProjectId;
    ProjectManager *pm;
    IssueManager *im;
};