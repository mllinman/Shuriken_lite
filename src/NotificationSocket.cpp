#include "NotificationSocket.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QTimer>

NotificationSocket::NotificationSocket(QString token, QObject *parent)
    : QObject(parent), token(token) 
{
    // Connect with JWT token as auth
    QUrl url("ws://localhost:4242");
    QUrlQuery query;
    socket.open(QUrl(url.toString() + "?token=" + token));

    connect(&socket, &QWebSocket::connected, this, &NotificationSocket::onConnected);
    connect(&socket, &QWebSocket::textMessageReceived, this, &NotificationSocket::onMessage);
}

void NotificationSocket::onConnected() {
    qDebug() << "🔌 Connected to notification server";
}

void NotificationSocket::onMessage(QString msg) {
    QJsonObject obj = QJsonDocument::fromJson(msg.toUtf8()).object();
    QString message = obj["message"].toString();
    emit newNotification(message);
}
void NotificationSocket::onDisconnected() {
    qDebug() << "🔌 Disconnected from notification server";
    reconnect();
}
void NotificationSocket::sendAuth() {
    QJsonObject authObj;
    authObj["type"] = "auth";
    authObj["token"] = token;
    QJsonDocument doc(authObj);
    socket.sendTextMessage(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}
void NotificationSocket::onError(QAbstractSocket::SocketError error) {
    qDebug() << "❗ WebSocket error:" << error;
    reconnect();
}
void NotificationSocket::reconnect() {
    if (reconnectAttempts >= maxReconnectAttempts) {
        qDebug() << "❗ Max reconnect attempts reached. Giving up.";
        return;
    }
    reconnectAttempts++;
    qDebug() << "🔄 Attempting to reconnect in" << reconnectIntervalMs / 1000 << "seconds...";
    if (!reconnectTimer) {
        reconnectTimer = new QTimer(this);
        connect(reconnectTimer, &QTimer::timeout, this, [this]() {
            qDebug() << "🔌 Reconnecting...";
            socket.open(QUrl("ws://localhost:4242?token=" + token));
            reconnectTimer->stop();
        });
    }
    reconnectTimer->start(reconnectIntervalMs);
}