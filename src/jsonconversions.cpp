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
