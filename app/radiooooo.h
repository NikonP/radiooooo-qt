#ifndef RADIOOOOO_H
#define RADIOOOOO_H

#include <QObject>
#include <QDir>
#include <QFile>
#include "configurator.h"

class Radiooooo : public QObject
{
    Q_OBJECT
public:
    explicit Radiooooo(QObject *parent = nullptr);

private:
    Configurator cfg;
    void firstLaunch();

signals:

public slots:
    QString loadConfig();
    //void playPause(bool state);
    //void nextSong();
    //void updateConfig(QString param, QString value, bool state);

};

#endif // RADIOOOOO_H
