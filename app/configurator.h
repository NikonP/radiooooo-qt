/*
 * This file is part of radiooooo-qt
 * GitHub: https://github.com/NikonP/radiooooo-qt
 * configurator.h - header file for configurator.cpp
 */

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QList>
#include <defaults.h>

class Configurator : public QObject
{
    Q_OBJECT
public:
    explicit Configurator(QObject *parent = nullptr);

    typedef QMap<QString, QList<QString>> ConfigStorage; // data type for config storage

    void initDirs(); // creates app's dirs
    void initConfig(); // init config with default values
    void saveConfig(); // saves config to json file
    void loadConfig(); // loads config from json file

    // updates param in config, adds value if enable==true, removes otherwise
    void updateConfig(QString param,
                      QString value,
                      bool enable);

    const QList<QString> moods = defMoods; // all available moods
    const QList<QString> allCountries = defCountries; // all available countries
    const int minDecade = 1910;
    const int maxDecade = 2020;

    ConfigStorage getConfig(); // config getter
    QString getConfigStr(); // config getter but it returns string
    QJsonDocument configToJson(ConfigStorage c); // converts config to json

    QString appDir = ".radiooooo-qt"; // name of the app's dir
    QString appDirPath = QDir::homePath() + "/" + appDir; // app's dir path
    QString configFilePath = appDirPath + "/config.json"; // app's config file path
    QString audioDirPath = appDirPath + "/" + "audio-files"; // dir for storing audio files

private:
    ConfigStorage config; // main config var

signals:

};

#endif // CONFIGURATOR_H
