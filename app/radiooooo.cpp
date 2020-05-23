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

    request.setUrl(QUrl(getCodesUrl + decade));
    request.setRawHeader("content-type", "application/json");

    QNetworkReply *reply = netManager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error() != QNetworkReply::NoError) {
        jsonObj.insert("error", reply->error());
        return jsonObj;
    }

    QString jsonStr = reply->readAll();
    jsonObj = QJsonDocument().fromJson(jsonStr.toUtf8()).object();

    reply->deleteLater();
    return jsonObj;
}

QJsonObject Radiooooo::getSongInfo() {
    QNetworkRequest request;
    QEventLoop loop;

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
    if(config["isocodes"].length() == 0 || config["isocodes"].contains("any")) {
        config["isocodes"] = cfg->allCountries;
    }

    // api requires uppercase
    for(QString& m : config["moods"]) {
        m = m.toUpper();
    }

    QJsonDocument jsonDoc = cfg->configToJson(config);

    QNetworkReply *reply = netManager->post(request, jsonDoc.toJson());
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString response = reply->readAll();
    QJsonObject info;
    if(reply->error() == QNetworkReply::NoError) {
        info = QJsonDocument().fromJson(response.toUtf8()).object();
    } else {
        info.insert("error", response);
    }

    reply->deleteLater();
    return info;
}

bool Radiooooo::saveFile(QString path, QByteArray data) {
    QFile binFile(path);

    if (!binFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    binFile.write(data);
    binFile.close();
    return true;
}

bool Radiooooo::downloadFile(QString filename, QString url) {
    QJsonObject jsonObj;
    QNetworkRequest request;
    QEventLoop loop;

    request.setUrl(QUrl(url));

    QNetworkReply *reply = netManager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error() != QNetworkReply::NoError) {
        return false;
    }

    QByteArray data = reply->readAll();
    bool success = saveFile(cfg->audioDirPath + "/" + filename, data);

    reply->deleteLater();
    return success;
}

void Radiooooo::playLoop() {
    // get song
    // wait while playing
    // ...
    // restart loop
    QJsonObject songInfo = getSongInfo();
    if(songInfo.keys().contains("error")) {
        qDebug() << "error: " << songInfo["error"];
        return;
    }

    QString title = songInfo["title"].toString();
    QString artist = songInfo["artist"].toString();
    QString year = songInfo["year"].toString();

    QString invalidChars = QRegExp::escape("\\/:*?\"\'<>| ");

    QString filename = title;
    filename.replace(QRegExp("[" + invalidChars + "]"), "_");
    filename += ".ogg";

    QString oggUrl = songInfo["links"].toObject()["ogg"].toString();
    qDebug() << title;
    qDebug() << artist;
    qDebug() << year;
    qDebug() << oggUrl;

    downloadFile(filename, oggUrl);

    if(oggUrl == "" ) {
        return;
    }

    state = PLAYING;
    try {
        while(true) {

            break;
        }
    } catch (const QString err) {
        // set error flag
    }
}

void Radiooooo::playPause(bool play) {
    playLoop();
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
