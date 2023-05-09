#include "gtest/gtest.h"
#include <string>
#include <vector>

#include "string.h"

TEST(vector, constructer)
{
    EXPECT_TRUE(true);
}

bool compare_vector(const std::vector<std::string> &_v1, const std::vector<std::string> &_v2)
{
    if (_v1.size() != _v2.size())
        return false;
    
    for (int i = 0; i < _v1.size(); ++i)
    {
        if (_v1[i] != _v2[i])
            return false;
    }

    return true;
}

TEST(string, split_simple)
{
    const std::string str = "A AA B A";
    const std::vector<std::string> expect = {"A", "AA", "B", "A"};

    const auto result = split(str, ' ');

    EXPECT_TRUE(compare_vector(result, expect));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
