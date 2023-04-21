#include "parser/include/tree.h"
#include <cstdlib>
#include <functional>

int main()
{
    tree<int, int> t;

    t.add_value<int>(std::rand(), 10);
    return 0;
}