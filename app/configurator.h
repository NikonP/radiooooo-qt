#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <defaults.h>

class Configurator : public QObject
{
    Q_OBJECT
public:
    explicit Configurator(QObject *parent = nullptr);

    typedef QMap<QString, QList<QString>> ConfigStorage;

    void initDirs();
    void initConfig();
    void saveConfig();
    void loadConfig();
    void updateConfig(QString param, QString value, bool enable);

    const QList<QString> moods = defMoods;
    const QList<QString> allCountries = defCountries;
    const int minDecade = 1910;
    const int maxDecade = 2020;

    ConfigStorage getConfig();
    QString getConfigStr();
    QJsonObject getConfigJson();
    QJsonDocument configToJson(ConfigStorage c);

    QString appDir = ".radiooooo-qt";
    QString appDirPath = QDir::homePath() + "/" + appDir;
    QString configFilePath = appDirPath + "/config.json";
    QString audioDirPath = appDirPath + "/" + "audio-files";

private:
    ConfigStorage config;

signals:

};

#endif // CONFIGURATOR_H
