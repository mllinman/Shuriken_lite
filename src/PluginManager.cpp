#include "PluginManager.h"
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

PluginManager::PluginManager(QString pluginDir) : pluginDir(pluginDir) {}

QList<Plugin> PluginManager::loadPlugins() {
    QList<Plugin> plugins;
    QDir dir(pluginDir);
    for (const QFileInfo &fi : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString jsonPath = fi.absoluteFilePath() + "/plugin.json";
        if (!QFile::exists(jsonPath)) continue;

        QFile file(jsonPath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            QJsonObject obj = doc.object();
            Plugin p;
            p.name = obj["name"].toString();
            p.command = obj["command"].toString();
            for (auto arg : obj["args"].toArray()) p.args << arg.toString();
            p.description = obj["description"].toString();
            plugins.append(p);
        }
    }
    return plugins;
}
