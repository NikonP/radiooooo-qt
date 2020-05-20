#include "radiooooo.h"
#include <QtDebug>
#include <QVariantMap>

Radiooooo::Radiooooo(QObject *parent) : QObject(parent)
{
    cfg = new Configurator();
    mediaPlayer = new QMediaPlayer();
    netManager = new QNetworkAccessManager(this);
}

void Radiooooo::firstLaunch() {
    cfg->initDirs();
    cfg->initConfig();
}

QString Radiooooo::loadConfig() {
    if(!QDir(cfg->appDirPath).exists()) {
        firstLaunch();
    } else {
        cfg->loadConfig();
    }

    QString jsonString = cfg->getConfigStr();
    return jsonString;
}

void Radiooooo::updateConfig(QString param, QString value, bool enable) {
    cfg->updateConfig(param, value, enable);
}

QJsonObject Radiooooo::getCountries(QString decade) {
    QJsonObject jsonObj;
    QNetworkRequest request;

    QEventLoop loop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    request.setUrl(QUrl(getCodesUrl + decade));
    request.setRawHeader("content-type", "application/json");
    QNetworkReply *reply = netManager->get(request);

    loop.exec();

    if(reply->error() != QNetworkReply::NoError) {
        jsonObj.insert("error", reply->error());
        return jsonObj;
    }

    QString jsonStr = reply->readAll();
    jsonObj = QJsonDocument().fromJson(jsonStr.toUtf8()).object();

    return jsonObj;
}

QJsonObject Radiooooo::getSongInfo() {
    QNetworkRequest request;

    QEventLoop loop;
    connect(netManager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    request.setUrl(QUrl(getSongUrl));
    request.setRawHeader("content-type", "application/json");

    Configurator::ConfigStorage config = cfg->getConfig();

    // change only local copy of config
    // use all if none selected
    if(config["moods"].length() == 0) {
        config["moods"] = cfg->moods;
    }
    // use all if none selected
    if(config["decades"].length() == 0) {
        for(int d = cfg->minDecade; d <= cfg->maxDecade; d+=10) {
            config["decades"].push_back(QString(d));
        }
    }

    // get all available if none selected
    if(config["countries"].length() == 0) {
        QSet<QVariant> uniqCodes;
        for(QString d : config["decades"]) {
            QJsonObject isocodes = getCountries(d);
            for(QString m : config["moods"]) {
                uniqCodes = isocodes[m].toArray().toVariantList().toSet();
            }
        }

        for(QVariant c : uniqCodes) {
            config["countries"].push_back(c.toString());
        }
    }

    for(QString& m : config["moods"]) {
        m = m.toUpper();
    }

    QJsonDocument jsonDoc = cfg->configToJson(config);

    QNetworkReply *reply = netManager->post(request, jsonDoc.toJson());
    loop.exec();

    qDebug() << reply->readAll();
}

QString Radiooooo::downloadFile(QString url) {
    // make get request to <url>
    // save file
    // return file path
}

void Radiooooo::playLoop() {
    // get song
    // wait while playing
    // ...
    // restart loop
    while(true) {

        break;
    }
}

void Radiooooo::playPause(bool play) {
    getSongInfo();
    if(play) {
        if(state == IDLE) {
            // download
            // play
        } else if(state == PAUSED) {
            // continue
        } else {
            // do nothing?
        }
    } else {
        if(state == PLAYING) {
            // pause
        }
    }
}

void Radiooooo::nextSong() {
    // download
    // play
}
