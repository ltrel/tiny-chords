#include <iostream>
#include <fmt/ranges.h>
#include "indexTransformations.h"

int main(int argc, char *argv[])
{
    std::cout << fmt::format("{}\n", flatten<3>({2, 3, 2}, {3, 4, 7}));
    std::cout << fmt::format("{}\n", collect<3>(35, {3, 4, 7}));
    return 0;
}
