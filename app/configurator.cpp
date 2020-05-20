#include "configurator.h"
#include <QtDebug>

Configurator::Configurator(QObject *parent) : QObject(parent)
{

}

QJsonDocument Configurator::configToJson(QMap<QString, QVector<QString>> c) {
    QJsonObject jsonObj;

    for(QString key : c.keys()) {
        QJsonArray jsonArr;
        for(QString val : c[key]) {
            jsonArr.push_back(val);
        }
        jsonObj.insert(key, jsonArr);
    }

    QJsonDocument jsonDoc(jsonObj);
    return jsonDoc;
}

void Configurator::initDirs() {
    QDir().mkdir(appDirPath);
    QDir().mkdir(oggDirPath);
    QDir().mkdir(mpegDirPath);
}

void Configurator::initConfig() {
    // init config with dafault values
    config["decades"].push_back("1980");  // select 1980 by default
    config["countries"].push_back("GBR"); // select United Kingdom by default
    config["moods"].push_back("Fast");    // select fast by default

    // save default config
    saveConfig();
}

void Configurator::saveConfig() {
    QJsonDocument jsonDoc = configToJson(config);

    QFile configFile(configFilePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    configFile.write(jsonDoc.toJson());
    configFile.close();
}

void Configurator::loadConfig() {
    QMap<QString, QVector<QString>> loadedConfig;

    QFile configFile(configFilePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        initConfig();
        return;
    }
    QJsonObject jsonObj = QJsonDocument().fromJson(configFile.readAll()).object();
    for(QString key : jsonObj.keys()) {
        for(QJsonValueRef val : jsonObj[key].toArray()) {
            loadedConfig[key].push_back(val.toString());
        }
    }

    config = loadedConfig;
}

QMap<QString, QVector<QString>>Configurator::getConfig() {
    return config;
}

QString Configurator::getConfigStr() {
    QJsonDocument jsonDoc = configToJson(config);
    QString jsonString = jsonDoc.toJson();
    qDebug() << jsonString;
    return jsonString;
}
