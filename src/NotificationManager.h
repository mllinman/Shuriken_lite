#include <qtmetamacros.h>
#include <QObject>
#include <QStringList>

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);

    QString userToken; // Add this member variable

signals:
    void notificationsListed(const QStringList &list);

public slots:
    void listNotifications();
};

