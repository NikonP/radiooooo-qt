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

    if(!QDir(cfg->appDirPath).exists() || !QDir(cfg->audioDirPath).exists()) {
        cfg->initDirs();
    }
    if(!QFileInfo::exists(cfg->configFilePath)) {
        cfg->initConfig();
    }
}

/*
 * Makes Configurator to load config from json file
 * Uses default config if file does not exist
 * @return QString - QString var as string (for qml)
 */
QString Radiooooo::loadConfig() {
    if(!QFileInfo::exists(cfg->configFilePath)) {
        cfg->initConfig();
    } else {
        cfg->loadConfig();
    }

    QString jsonString = cfg->getConfigStr();
    return jsonString;
}

/*
 * Calls Configurator updateConfig function
 */
void Radiooooo::updateConfig(QString param, QString value, bool enable) {
    cfg->updateConfig(param, value, enable);
}

/*
 * Makes GET request to isocodes endpoint
 * @param QString decade - decades 1910-2020 with step 10
 * @return QJsonObject - api response or {"error": {err_msg}} on error
 */
QJsonObject Radiooooo::getCountries(QString decade) {
    QJsonObject jsonObj;
    QNetworkRequest request;
    QEventLoop loop; // loop for synchronous request

    request.setUrl(QUrl(getCodesUrl + decade)); // api endpoint
    request.setRawHeader("content-type", "application/json"); // use json

    QNetworkReply *reply = netManager->get(request);
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString jsonStr = reply->readAll(); // read response

    // return {"error": {err_msg}} on request error
    if(reply->error() != QNetworkReply::NoError) {
        jsonObj.insert("error", jsonStr);
        return jsonObj;
    }

    jsonObj = QJsonDocument().fromJson(jsonStr.toUtf8()).object(); // convert to json

    reply->deleteLater();
    return jsonObj;
}

/*
 * Makes POST request to get-song endpoint
 * POST structure - same as config
 * fun radiooooo.app bug: if use all isocodes always returns "New York Business"
 * @return QJsonObject - api response or {"error": {err_msg}} on error
 */
QJsonObject Radiooooo::getSongInfo() {
    QNetworkRequest request;
    QEventLoop loop; // loop for synchronous request

    request.setUrl(QUrl(getSongUrl)); // api endpoint
    request.setRawHeader("content-type", "application/json"); // use json

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
    qDebug() << config;

    // api requires uppercase
    for(QString& m : config["moods"]) {
        m = m.toUpper();
    }

    QJsonDocument jsonDoc = cfg->configToJson(config);

    QNetworkReply *reply = netManager->post(request, jsonDoc.toJson());
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString response = reply->readAll(); // read response
    QJsonObject info;

    // return {"error": {err_msg}} on request error
    if(reply->error() != QNetworkReply::NoError) {
        info.insert("error", response);
    }

    info = QJsonDocument().fromJson(response.toUtf8()).object(); // convert to json

    reply->deleteLater();
    return info;
}

/*
 * Saves binary file (song)
 * @param QString path - where to save
 * @param QByteArray data - some bin data
 * @return bool - true on success, false otherwise
 */
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

/*
 * Downloads song from given url to audio-dir
 * @param QString filename - name of new file
 * @param QString url
 * @return QString - full path to new file or "error" string
 */
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

/*
 * Downloads new file and makes QMediaPlayer to play it
 */
void Radiooooo::playNext() {
    state = IDLE; // update state
    mediaPlayer->stop();

    QJsonObject songInfo = getSongInfo(); // get new song
    if(songInfo.keys().contains("error")) {
        qDebug() << "error: " << songInfo["error"];
        return;
    }

    // parse response data
    QString title = songInfo["title"].toString();
    QString artist = songInfo["artist"].toString();
    QString year = songInfo["year"].toString();

    nowPlaying = QString("\"%1\" by \"%2\". %3").arg(title).arg(artist).arg(year);

    // make valid filename
    QString invalidChars = QRegExp::escape("\\/:*?\"\'<>| ");
    QString filename = title;
    filename.replace(QRegExp("[" + invalidChars + "]"), "_");
    filename += ".ogg";

    // get direct link to audio file
    QString oggUrl = songInfo["links"].toObject()["ogg"].toString();
    if(oggUrl == "") {
        return;
    }

    // donwload file
    QString filePath = downloadFile(filename, oggUrl);
    if(filePath == "error") {
        return;
    }

    state = PLAYING; // update state

    // make QMediaPlayer play new song
    mediaPlayer->setMedia(QUrl::fromLocalFile(filePath));
    mediaPlayer->play();

    updateStatusMsg(nowPlaying);
}

/*
 * Starts next song when prev ends
 * @param State playerState - QMediaPlayer state
 * slot for QMediaPlayer signal
 */
void Radiooooo::stateChanged(QMediaPlayer::State playerState) {
    if(playerState == QMediaPlayer::StoppedState && state != IDLE) {
        playNext();
    }
}

/*
 * Updates audioDuration
 * @param qint64 newDuration - new song duration
 * slot for QMediaPlayer signal
 */
void Radiooooo::durationChanged(qint64 newDuration) {
    audioDuration = newDuration;
}

/*
 * Calcs progress status and calls updateProgressBar
 * @param qint64 pos - current QMediaPlayer position
 * slot for QMediaPlayer signal
 */
void Radiooooo::updateProgress(qint64 pos) {
    double progress;
    if(audioDuration == 0) {
        progress = 0;
    } else {
        progress = (double) pos / (double) audioDuration;
    }

    updateProgressBar(progress);
}

/*
 * Sets QMediaPlayer volume
 * @param int volume - new QMediaPlayer volume
 */
void Radiooooo::setVolume(int volume) {
    mediaPlayer->setVolume(volume);
}

/*
 * Makes audio player to pause or play
 * @param bool play - pause (false) or play (true)
 */
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

/*
 * Makes audio player to skip song
 */
void Radiooooo::nextSong() {
    if(state == PLAYING) {
        playNext();
    }
}
