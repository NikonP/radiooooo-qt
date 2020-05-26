/*
 * This file is part of radiooooo-qt
 * radiooooo.cpp - implements interaction with https://radiooooo.app/
 * and audio player logic.
 */

#include "radiooooo.h"

Radiooooo::Radiooooo(QObject *parent) : QObject(parent)
{
    cfg = new Configurator();
    mediaPlayer = new QMediaPlayer();
    connect(mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateProgress(qint64)));
    connect(mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));
    connect(mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));

    netManager = new QNetworkAccessManager(this);
}

void Radiooooo::firstLaunch() {
    cfg->initDirs();
    cfg->initConfig();
}

QString Radiooooo::loadConfig() {
    if(!QDir(cfg->appDirPath).exists() || !QDir(cfg->audioDirPath).exists()) {
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
            config["decades"].push_back(QString::number(d));
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
    if(QFileInfo(path).exists()) {
        return true;
    }

    QFile binFile(path);

    if (!binFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    binFile.write(data);
    binFile.close();
    return true;
}

QString Radiooooo::downloadFile(QString filename, QString url) {
    updateStatusMsg("Downloading new song...");

    QJsonObject jsonObj;
    QNetworkRequest request;
    QEventLoop loop;

    request.setUrl(QUrl(url));

    QNetworkReply *reply = netManager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    if(reply->error() != QNetworkReply::NoError) {
        return "error";
    }

    QByteArray data = reply->readAll();
    QString filePath = cfg->audioDirPath + "/" + filename;
    bool success = saveFile(filePath, data);

    if(!success) {
        filePath = "error";
    }

    reply->deleteLater();
    return filePath;
}

void Radiooooo::playNext() {
    state = IDLE;
    mediaPlayer->stop();

    QJsonObject songInfo = getSongInfo();
    if(songInfo.keys().contains("error")) {
        qDebug() << "error: " << songInfo["error"];
        return;
    }

    QString title = songInfo["title"].toString();
    QString artist = songInfo["artist"].toString();
    QString year = songInfo["year"].toString();

    nowPlaying = QString("\"%1\" by \"%2\". %3").arg(title).arg(artist).arg(year);

    QString invalidChars = QRegExp::escape("\\/:*?\"\'<>| ");

    QString filename = title;
    filename.replace(QRegExp("[" + invalidChars + "]"), "_");
    filename += ".ogg";

    QString oggUrl = songInfo["links"].toObject()["ogg"].toString();
    if(oggUrl == "") {
        return;
    }

    QString filePath = downloadFile(filename, oggUrl);
    if(filePath == "error") {
        return;
    }

    state = PLAYING;
    mediaPlayer->setMedia(QUrl::fromLocalFile(filePath));
    mediaPlayer->setVolume(50);
    mediaPlayer->play();
    updateStatusMsg(nowPlaying);
}

void Radiooooo::stateChanged(QMediaPlayer::State playerState) {
    if(playerState == QMediaPlayer::StoppedState && state != IDLE) {
        playNext();
    }
}

void Radiooooo::durationChanged(qint64 newDuration) {
    audioDuration = newDuration;
}

void Radiooooo::updateProgress(qint64 pos) {
    double progress;
    if(audioDuration == 0) {
        progress = 0;
    } else {
        progress = (double) pos / (double) audioDuration;
    }

    updateProgressBar(progress);
}

void Radiooooo::setVolume(int volume) {
    mediaPlayer->setVolume(volume);
}

void Radiooooo::playPause(bool play) {
    if(play) {
        if(state == IDLE) {
            playNext();
        } else if(state == PAUSED) {
            state = PLAYING;
            mediaPlayer->play();
            updateStatusMsg(nowPlaying);
        }
    } else {
        if(state == PLAYING) {
            state = PAUSED;
            mediaPlayer->pause();
            updateStatusMsg("Paused");
        }
    }
}

void Radiooooo::nextSong() {
    if(state == PLAYING) {
        playNext();
    }
}
