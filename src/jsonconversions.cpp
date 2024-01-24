#include <nlohmann/json.hpp>
#include "section.hpp"
#include "enums.hpp"
#include "chord.hpp"

using json = nlohmann::json;

void to_json(json &j, const Chord &chord)
{
    j["root"] = chord.root();
    j["chordType"] = ChordTypeUtils::serialize(chord.chordType());
    j["beats"] = chord.beats();
    if (!chord.bass().empty()) j["bass"] = chord.bass();
}

void to_json(json &j, const Section &section)
{
    j["beatsPerBar"] = section.header.beatCount();
    j["beatType"] = BeatTypeUtils::serialize(section.header.beatType());
    j["bpm"] = section.header.approxBpm();

    j["chords"] = section.chords;
}
