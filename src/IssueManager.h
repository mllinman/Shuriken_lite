#pragma once
#include <QObject>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class IssueManager : public QObject {
    Q_OBJECT
public:
    explicit IssueManager(QObject *parent = nullptr);
    ~IssueManager();

    void listIssues(int projectId);
    void createIssue(int projectId, QString title, QString body);
    void updateIssueStatus(int issueId, QString status);
    void listComments(int issueId);
    void addComment(int issueId, QString body);


signals:
    void issuesListed(QStringList issues);
    void issueCreated(QString title);
    void issueUpdated(int issueId, QString status);
    void errorOccurred(QString error);
    void commentsListed(QStringList comments);
    void commentAdded(QString body);
private:
    QNetworkAccessManager *manager;
    QString apiUrl;
    QStringList comments;
    QStringList issueTitles;
    QStringList projectNames;
    QStringList userNames;
    QStringList issueStatuses;
    QStringList issueComments;

};
