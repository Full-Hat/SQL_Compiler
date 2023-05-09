#include "gtest/gtest.h"
#include <ostream>
#include <vector>

std::ostream& operator<<(std::ostream &out, const std::vector<int> &_v)
{
    return out;
}

class A
{
public:
    static int aba(std::vector<int> a)
    {
        return 0;
    }
};

int main(int argc, char *argv[])
{
    std::vector<int> a;
    std::cout << a;
    A::aba({});
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}