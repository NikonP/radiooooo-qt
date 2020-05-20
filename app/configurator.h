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
#include <QMap>
#include <QList>

class Configurator : public QObject
{
    Q_OBJECT
public:
    explicit Configurator(QObject *parent = nullptr);
    void initDirs();
    void initConfig();
    void saveConfig();
    void loadConfig();
    void updateConfig(QString param, QString value, bool enable);

    const QList<QString> moods = {"Fast", "Slow", "Weird"};
    const int minDecade = 1910;
    const int maxDecade = 2020;

    QMap<QString, QList<QString>> getConfig();
    QString getConfigStr();

    QString appDir = ".radiooooo-qt";
    QString appDirPath = QDir::homePath() + "/" + appDir;
    QString configFilePath = appDirPath + "/config.json";
    QString oggDirPath = appDirPath + "/" + "ogg-files";
    QString mpegDirPath = appDirPath + "/" + "mpeg-files";

private:
    QMap<QString, QList<QString>> config;
    QJsonDocument configToJson(QMap<QString, QList<QString>> c);

signals:

};

#endif // CONFIGURATOR_H
