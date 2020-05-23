#ifndef RADIOOOOO_H
#define RADIOOOOO_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QIODevice>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrlQuery>
#include <QSet>
#include <QVariant>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QRegExp>
#include "configurator.h"

class Radiooooo : public QObject
{
    Q_OBJECT
public:
    explicit Radiooooo(QObject *parent = nullptr);
    enum {IDLE, PLAYING, PAUSED, DOWNLOADING}; // states
    int state = IDLE;

private:
    const QString baseAPI = "https://radiooooo.app";
    const QString getCodesUrl = baseAPI + "/country/mood?decade=";
    const QString getSongUrl = baseAPI + "/play";

    Configurator *cfg;
    QMediaPlayer *mediaPlayer;
    QNetworkAccessManager *netManager;
    void firstLaunch(); // init default stuff on first launch

    QJsonObject getSongInfo(); // returns json data from Radiooooo (song name, url, etc)
    QJsonObject getCountries(QString decade); // gets available isocodes
    bool downloadFile(QString path, QString url); // downloads audio file from url, returns true if success
    bool saveFile(QString path, QByteArray data); // saves audio file, returns true if success

signals:


public slots:
    QString loadConfig(); // loading config from json file
    void updateConfig(QString param, QString value, bool enable);

    void playPause(bool play);
    void nextSong();
    void playLoop();

};

#endif // RADIOOOOO_H
