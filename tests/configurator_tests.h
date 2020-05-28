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

#endif // CONFIGURATOR_TESTS_H
