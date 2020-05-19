#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class Configurator : public QObject
{
    Q_OBJECT
public:
    explicit Configurator(QObject *parent = nullptr);
    void initDirs();
    void initConfig();
    void saveConfig();
    void loadConfig();
    QString appDir = ".radiooooo-qt";
    QString appDirPath = QDir::homePath() + "/" + appDir;
    QString configFilePath = appDirPath + "/config.json";
    QString oggDirPath = appDirPath + "/" + "ogg-files";
    QString mpegDirPath = appDirPath + "/" + "mpeg-files";

private:
    QHash<QString, QVector<QString>> config;
    QJsonDocument configToJson(QHash<QString, QVector<QString>> c);

signals:

};

#endif // CONFIGURATOR_H
