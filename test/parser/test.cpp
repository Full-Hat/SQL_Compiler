#include "gtest/gtest.h"

#include "Containers/test.h"

TEST(vector, constructer)
{
    EXPECT_TRUE(true);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}