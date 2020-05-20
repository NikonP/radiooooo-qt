#include "radiooooo.h"
#include <QtDebug>
#include <QVariantMap>

Radiooooo::Radiooooo(QObject *parent) : QObject(parent)
{

}

void Radiooooo::firstLaunch() {
    cfg.initDirs();
    cfg.initConfig();
}

QString Radiooooo::loadConfig() {
    if(!QDir(cfg.appDirPath).exists()) {
        firstLaunch();
    } else {
        cfg.loadConfig();
    }

    QString jsonString = cfg.getConfigStr();
    return jsonString;
}

void Radiooooo::updateConfig(QString param, QString value, bool enable) {
    cfg.updateConfig(param, value, enable);
}
