#include "Utils/Containers/tree.h"
#include <cstdlib>
#include <functional>
#include <vector>

int main()
{
    tree<int, int> t;

    t.add_value<int>(std::rand(), 10);
    return 0;
}