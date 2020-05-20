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
    void firstLaunch();

    QJsonObject getSongInfo();
    QJsonObject getCountries(QString decade); // get available isocodes
    QString downloadFile(QString url); // returns file path

signals:


public slots:
    QString loadConfig();
    void updateConfig(QString param, QString value, bool enable);

    void playPause(bool play);
    void nextSong();
    void playLoop();

};

#endif // RADIOOOOO_H
