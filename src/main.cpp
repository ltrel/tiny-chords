#include <iostream>
#include <fmt/ranges.h>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "enums.hpp"
#include "chord.hpp"
#include "note.hpp"
#include "sectionheader.hpp"
#include "section.hpp"
#include "jsonconversions.hpp"
using json = nlohmann::json;

// int main(int argc, char *argv[])
int main()
{
    // std::cout << fmt::format("{}\n", flatten<3>({2, 3, 2}, {3, 4, 7}));
    // std::cout << fmt::format("{}\n", collect<3>(35, {3, 4, 7}));

    // std::stringstream headerStream{};
    // SectionHeader writeHeader{138, BeatType::quarter, 4};
    // writeHeader.write(headerStream);
    // SectionHeader readHeader{SectionHeader::read(headerStream)};

    // Chord writeChord{"F#", ChordType::maj7, 4, "Ab"};
    // std::cout << json{writeChord} << '\n';

    // std::ofstream outStream{"out.tc", std::ios::binary | std::ios::out};
    // writeChord.write(4, outStream);
    // outStream.close();

    // std::ifstream inStream{"out.tc", std::ios::binary | std::ios::in};
    // Chord readChord{Chord::read(4, inStream)};
    // std::cout << readChord.print() << '\n';
    // inStream.close();

    std::ofstream outStream{"out.tc", std::ios::binary | std::ios::out};
    SectionHeader header{120, BeatType::quarter, 4};
    Section section{header, {
        {Note("C#"), ChordType::min7, header.beatCount(), Note("E")},
        {Note("F#"), ChordType::dom7, header.beatCount()},
        {Note("B"), ChordType::maj7, header.beatCount()},
    }};
    std::cout << json{section}.dump(4) << '\n';
    section.write(outStream);
    outStream.close();

    std::ifstream inStream{"out.tc", std::ios::binary | std::ios::in};
    Section readSection{Section::read(inStream)};
    for (auto &chord : readSection.chords)
    {
        std::cout << chord.print() << '\n';
    }
    inStream.close();

    return 0;
}
