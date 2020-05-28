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

TEST(update_config, add_val) {
    Configurator cfg;
    cfg.initDirs();
    cfg.initConfig(); // load default

    cfg.updateConfig("moods", "TEST_VAL_1", true);
    cfg.updateConfig("isocodes", "TEST_VAL_2", true);
    cfg.updateConfig("decades", "TEST_VAL_3", true);

    Configurator::ConfigStorage config = cfg.getConfig();

    EXPECT_EQ(1, config["moods"].contains("TEST_VAL_1"));
    EXPECT_EQ(1, config["isocodes"].contains("TEST_VAL_2"));
    EXPECT_EQ(1, config["decades"].contains("TEST_VAL_3"));
}

TEST(update_config, remove_val) {
    Configurator cfg;
    cfg.initDirs();
    cfg.initConfig(); // load default

    cfg.updateConfig("moods", "TEST_VAL_1", true);
    cfg.updateConfig("isocodes", "TEST_VAL_2", true);
    cfg.updateConfig("decades", "TEST_VAL_3", true);

    cfg.updateConfig("moods", "TEST_VAL_1", false);
    cfg.updateConfig("isocodes", "TEST_VAL_2", false);
    cfg.updateConfig("decades", "TEST_VAL_3", false);

    Configurator::ConfigStorage config = cfg.getConfig();

    EXPECT_EQ(0, config["moods"].contains("TEST_VAL_1"));
    EXPECT_EQ(0, config["isocodes"].contains("TEST_VAL_2"));
    EXPECT_EQ(0, config["decades"].contains("TEST_VAL_3"));
}

#endif // CONFIGURATOR_TESTS_H
