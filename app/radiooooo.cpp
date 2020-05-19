#include "radiooooo.h"
#include <QtDebug>

Radiooooo::Radiooooo(QObject *parent) : QObject(parent)
{

}

void Radiooooo::firstLaunch() {
    cfg.initDirs();
    cfg.initConfig();
}

void Radiooooo::loadConfig() {
    if(!QDir(cfg.appDirPath).exists()) {
        firstLaunch();
    } else {
        cfg.loadConfig();
    }
}
