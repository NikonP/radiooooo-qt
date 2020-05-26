/*
 * This file is part of radiooooo-qt
 * radiooooo.h - header file for radiooooo.cpp
 */

#ifndef RADIOOOOO_H
#define RADIOOOOO_H

#include <QObject>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include "configurator.h"

/*
 * Radiooooo class
 * implements interaction with https://radiooooo.app/ and audio player logic
 */
class Radiooooo : public QObject
{
    Q_OBJECT
public:
    explicit Radiooooo(QObject *parent = nullptr);
    enum {IDLE, PLAYING, PAUSED, DOWNLOADING}; // audio player states
    int state = IDLE; // current state

private:
    const QString baseAPI = "https://radiooooo.app"; // api root url
    const QString getCodesUrl = baseAPI + "/country/mood?decade="; // get isocodes endpoint
    const QString getSongUrl = baseAPI + "/play"; // get songs endpoint
    QString nowPlaying; // currently playing <song name> + <artist>
    qint64 audioDuration = 0; // current song duration (for progress bar)

    Configurator *cfg; // configuration stuff
    QMediaPlayer *mediaPlayer; // actually media player :/
    QNetworkAccessManager *netManager; // network stuff
    void firstLaunch(); // init default stuff on first launch

    QJsonObject getSongInfo(); // returns json data from Radiooooo (song name, url, etc)
    QJsonObject getCountries(QString decade); // gets available isocodes
    QString downloadFile(QString path, QString url); // downloads audio file from url, returns filepath if success
    bool saveFile(QString path, QByteArray data); // saves audio file, returns true if success

    void playNext(); // Downloads new file and makes QMediaPlayer to play it

signals:
    void updateProgressBar(double progress); // 0 - 0%, 1 - 100%
    void updateStatusMsg(QString message); // set status message

public slots:
    QString loadConfig(); // loading config from json file

    // updates param in config, adds value if enable==true, removes otherwise
    void updateConfig(QString param,
                      QString value,
                      bool enable);

    void playPause(bool play); // makes audio player to pause or play
    void nextSong(); // makes audio player to skip song
    void setVolume(int volume); // sets audio player volume

    void stateChanged(QMediaPlayer::State playerState); // starts next song when prev ends
    void durationChanged(qint64 newDuration); // updates audioDuration
    void updateProgress(qint64 pos); // calcs progress status and calls updateProgressBar

};

#endif // RADIOOOOO_H
