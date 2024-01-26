#include <iostream>
#include <fmt/ranges.h>
#include <string_view>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "enums.hpp"
#include "chord.hpp"
#include "note.hpp"
#include "sectionheader.hpp"
#include "section.hpp"
#include "jsonconversions.hpp"
using json = nlohmann::ordered_json;

static constexpr std::string_view argumentErrStr{"Please specify a .json or .tc file\n"};

// int main(int argc, char *argv[])
int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        std::cout << argumentErrStr;
        return 0;
    }

    std::string_view fileArg{argv[1]};
    if (fileArg.ends_with(".json"))
    {
        std::ifstream inStream{argv[1]};
        json data(json::parse(inStream));
        Section section{data.template get<Section>()};
        inStream.close();

        std::string outName{fileArg.substr(0, fileArg.find_last_of('.'))};
        outName.append(".tc");
        std::ofstream outStream{outName};
        section.write(outStream);
        outStream.close();
        std::cout << fmt::format("Wrote output to: {}\n", outName);
        return 0;
    }

    if (fileArg.ends_with(".tc"))
    {
        std::ifstream inStream{argv[1]};
        Section section{Section::read(inStream)};
        json j(section);
        std::cout << j.dump(2) << '\n';
        inStream.close();
        return 0;
    }

    std::cout << argumentErrStr;
    return 0;

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

    // std::ofstream outStream{"out.tc", std::ios::binary | std::ios::out};
    // SectionHeader header{120, BeatType::quarter, 4};
    // Section section{header, {
    //     {Note("C#"), ChordType::min7, header.beatCount(), Note("E")},
    //     {Note("F#"), ChordType::dom7, header.beatCount()},
    //     {Note("B"), ChordType::maj7, header.beatCount()},
    // }};
    // json j(section);
    // std::cout << j.dump(4) << '\n';
    // section.write(outStream);
    // outStream.close();

    // std::ifstream inStream{"out.tc", std::ios::binary | std::ios::in};
    // Section readSection{Section::read(inStream)};
    // for (auto &chord : readSection.chords)
    // {
    //     std::cout << chord.print() << '\n';
    // }
    // inStream.close();

    // std::ifstream jsonIStream{"test.json"};
    // json jread;
    // jsonIStream >> jread;
    // Section jsonReadSection{jread.template get<Section>()};
    // for (auto &chord : jsonReadSection.chords)
    // {
    //     std::cout << chord.print() << '\n';
    // }

    // return 0;
}
