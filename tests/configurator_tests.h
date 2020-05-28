#ifndef CONFIGURATOR_TESTS_H
#define CONFIGURATOR_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QDir>
#include "configurator.h"

using namespace testing;

TEST(init_dirs, check_dirs)
{
    Configurator cfg;
    cfg.initDirs();
    EXPECT_EQ(true, QDir(cfg.appDirPath).exists());
    EXPECT_EQ(true, QDir(cfg.audioDirPath).exists());
}

TEST(init_config, check_config) {
    Configurator cfg;
    cfg.initDirs();
    cfg.initConfig();
    Configurator::ConfigStorage config = cfg.getConfig();
    for(auto key : cfg.defaultConfig.keys()) {
        EXPECT_EQ(1, config.keys().contains(key));
        EXPECT_EQ(1, config[key].contains(cfg.defaultConfig[key]));
    }
}

#endif // CONFIGURATOR_TESTS_H
