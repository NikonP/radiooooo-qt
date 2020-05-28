#ifndef CONFIGURATOR_TESTS_H
#define CONFIGURATOR_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <QDir>
#include <QFileInfo>
#include "configurator.h"

using namespace testing;

TEST(init_dirs, check_dirs)
{
    Configurator cfg;
    cfg.initDirs();
    EXPECT_EQ(true, QDir(cfg.appDirPath).exists());
    EXPECT_EQ(true, QDir(cfg.audioDirPath).exists());
}

TEST(init_config, check_data) {
    Configurator cfg;
    cfg.initDirs();
    cfg.initConfig(); // load default
    Configurator::ConfigStorage config = cfg.getConfig();
    for(auto key : cfg.defaultConfig.keys()) {
        EXPECT_EQ(1, config.keys().contains(key));
        EXPECT_EQ(1, config[key].length());
        EXPECT_EQ(1, config[key].contains(cfg.defaultConfig[key]));
    }
}

TEST(save_config, check_file) {
    Configurator cfg;
    cfg.initDirs();
    cfg.initConfig(); // load default
    cfg.saveConfig(); // save deafult

    QFileInfo test_file(cfg.configFilePath);
    EXPECT_EQ(1, test_file.exists());
    EXPECT_EQ(1, test_file.isFile());
}

TEST(load_config, check_data) {
    Configurator cfg;
    cfg.initDirs();
    cfg.initConfig(); // load default
    cfg.saveConfig(); // save deafult

    Configurator::ConfigStorage config = cfg.getConfig();
    EXPECT_EQ(1, config.keys().length() != 0);
    for(auto key : cfg.defaultConfig.keys()) {
        EXPECT_EQ(1, config.keys().contains(key));
        EXPECT_EQ(1, config[key].length());
        EXPECT_EQ(1, config[key].contains(cfg.defaultConfig[key]));
    }
}

#endif // CONFIGURATOR_TESTS_H
