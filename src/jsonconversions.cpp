#include <utility>
#include <nlohmann/json.hpp>
#include "section.hpp"
#include "enums.hpp"
#include "chord.hpp"

using json = nlohmann::ordered_json;

void to_json(json &j, const Chord &chord)
{
    j["root"] = chord.root().print();
    j["chordType"] = ChordTypeUtils::serialize(chord.chordType());
    j["beats"] = chord.beats();
    if (chord.bass().has_value()) j["bass"] = chord.bass()->print();
}

void to_json(json &j, const Section &section)
{
    j["beatsPerBar"] = section.header.beatCount();
    j["beatType"] = BeatTypeUtils::serialize(section.header.beatType());
    j["bpm"] = section.header.approxBpm();

    j["chords"] = section.chords;
}

void from_json(const json &j, Chord &chord)
{
    chord.setRoot({j.at("root").template get<std::string>()});
    std::string_view chordTypeStr{j.at("chordType").template get<std::string>()};
    chord.setChordType({ChordTypeUtils::deserialize(chordTypeStr)});
    chord.setBeats({j.at("beats").template get<int>()});
    if (j.contains("bass")) chord.setBass(Note{j.at("bass").template get<std::string>()});
}

void from_json(const json &j, Section &section)
{
    section.header.setBeatCount({j.at("beatsPerBar").template get<int>()});
    std::string_view beatTypeStr{j.at("beatType").template get<std::string>()};
    section.header.setBeatType({BeatTypeUtils::deserialize(beatTypeStr)});
    section.header.setApproxBpm({j.at("bpm").template get<int>()});
    auto chords{j.at("chords").get_to(section.chords)};
}