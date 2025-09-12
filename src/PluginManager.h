#pragma once
#include <QString>
#include <QList>

struct Plugin {
    QString name;
    QString command;
    QStringList args;
    QString description;
};

class PluginManager {
public:
    PluginManager(QString pluginDir);
    QList<Plugin> loadPlugins();

private:
    QString pluginDir;
};
