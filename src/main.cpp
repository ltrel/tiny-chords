#include <iostream>
#include <fmt/ranges.h>
#include <fstream>
#include <bitset>
#include <sstream>
#include "indextransformations.hpp"
#include "chordtype.hpp"
#include "chunks.hpp"

int main(int argc, char *argv[])
{
    std::cout << fmt::format("{}\n", flatten<3>({2, 3, 2}, {3, 4, 7}));
    std::cout << fmt::format("{}\n", collect<3>(35, {3, 4, 7}));

    std::stringstream headerStream{};
    SectionHeader writeHeader{138, BeatType::quarter, 4};
    writeHeader.write(headerStream);
    SectionHeader readHeader{SectionHeader::read(headerStream)};

    Chord writeChord{"Ab", ChordType::min, 4};
    std::ofstream outStream{"out.tc", std::ios::binary | std::ios::out};
    writeChord.write(4, outStream);
    outStream.close();

    std::ifstream inStream{"out.tc", std::ios::binary | std::ios::in};
    Chord readChord{Chord::read(4, inStream)};
    return 0;
}
