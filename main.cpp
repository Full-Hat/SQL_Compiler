#include "Utils/Containers/tree.h"
#include <cstdlib>
#include <functional>
#include <vector>

class A
{
public:
    A() = default;

    A(A &&a) = default;
    //A(const A &a)  = default;
};

int main()
{
    std::vector<A> vec;
    A &&a = A();
    vec.push_back(std::move(a));

    //tree<int, int> t;

    //t.add_value<int>(std::rand(), 10);
    return 0;
}