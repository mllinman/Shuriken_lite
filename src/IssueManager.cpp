#include "IssueManager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

extern QString globalUserToken;

IssueManager::IssueManager(QObject *parent) : QObject(parent) {}

void IssueManager::listIssues(int projectId) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://localhost:4242/issues/" + QString::number(projectId)));

    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error()) {
            emit errorOccurred(reply->errorString());
        } else {
            QStringList list;
            QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
            for (auto v : arr) {
                QJsonObject obj = v.toObject();
                QString item = QString("[%1] %2 (%3)")
                                   .arg(obj["status"].toString())
                                   .arg(obj["title"].toString())
                                   .arg(obj["creatorEmail"].toString());
                list << item;
            }
            emit issuesListed(list);
        }
        reply->deleteLater();
    });
}

void IssueManager::createIssue(int projectId, QString title, QString body) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://localhost:4242/issues/create"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer " + globalUserToken.toUtf8());

    QJsonObject payload;
    payload["projectId"] = projectId;
    payload["title"] = title;
    payload["body"] = body;

    QNetworkReply *reply = manager->post(req, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (!reply->error()) {
            emit issueCreated(title);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}

void IssueManager::updateIssueStatus(int issueId, QString status) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://localhost:4242/issues/update"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer " + globalUserToken.toUtf8());

    QJsonObject payload;
    payload["issueId"] = issueId;
    payload["status"] = status;

    QNetworkReply *reply = manager->post(req, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (!reply->error()) {
            emit issueUpdated(issueId, status);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}
void IssueManager::listComments(int issueId) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://localhost:4242/issues/" + QString::number(issueId) + "/comments"));

    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error()) {
            emit errorOccurred(reply->errorString());
        } else {
            QStringList list;
            QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
            for (auto v : arr) {
                QJsonObject obj = v.toObject();
                QString comment = QString("💬 %1: %2")
                                      .arg(obj["commenterEmail"].toString())
                                      .arg(obj["body"].toString());
                list << comment;
            }
            emit commentsListed(list);
        }
        reply->deleteLater();
    });
}

void IssueManager::addComment(int issueId, QString body) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest req(QUrl("http://localhost:4242/issues/comment"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer " + globalUserToken.toUtf8());

    QJsonObject payload;
    payload["issueId"] = issueId;
    payload["body"] = body;

    QNetworkReply *reply = manager->post(req, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (!reply->error()) {
            emit commentAdded(body);
        } else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    });
}
