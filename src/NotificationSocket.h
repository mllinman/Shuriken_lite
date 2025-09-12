#pragma once
#include <QObject>
#include <QWebSocket>

class NotificationSocket : public QObject {
    Q_OBJECT
public:
    explicit NotificationSocket(QString token, QObject *parent = nullptr);

signals:
    void newNotification(QString msg);

private slots:
    void onConnected();
    void onMessage(QString msg);

private:
    QWebSocket socket;
    QString token;
    void onDisconnected();
    void sendAuth();
    void onError(QAbstractSocket::SocketError error);
    void reconnect();
    QTimer *reconnectTimer;
    int reconnectAttempts = 0;
    const int maxReconnectAttempts = 5;
    const int reconnectIntervalMs = 5000; // 5 seconds
};