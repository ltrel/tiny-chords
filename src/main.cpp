#include <iostream>
#include <fmt/ranges.h>
#include <bitset>
#include "indexTransformations.hpp"
#include "chunks.hpp"

int main(int argc, char *argv[])
{
    std::cout << fmt::format("{}\n", flatten<3>({2, 3, 2}, {3, 4, 7}));
    std::cout << fmt::format("{}\n", collect<3>(35, {3, 4, 7}));
    SectionHeader header{138, BeatType::quarter, 4};
    std::cout << std::bitset<16>{header.encode()};
    SectionHeader convert{SectionHeader::decode(header.encode())};
    return 0;
}
