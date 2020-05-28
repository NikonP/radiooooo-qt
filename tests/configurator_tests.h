#ifndef CONFIGURATOR_TESTS_H
#define CONFIGURATOR_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

using namespace testing;

TEST(test11, case11)
{
    EXPECT_EQ(1, 1);
    ASSERT_THAT(0, Eq(0));
}

#endif // CONFIGURATOR_TESTS_H
