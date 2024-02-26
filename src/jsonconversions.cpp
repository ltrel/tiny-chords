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

    j["chords"] = json::array();
    // Points to the last object in the JSON array
    int jsonIndex{-1};
    for (int i{0}; i < section.chords.size(); ++i)
    {
        const Chord &next{section.chords[i]};
        if (i > 0)
        {
            const Chord &prev{section.chords[i - 1]};
            // Combine adjacent chords that are the same aside from duration
            bool sameRoot{next.root() == prev.root()};
            bool sameType{next.chordType() == prev.chordType()};
            bool sameBass{next.bass() == prev.bass()};
            if (sameRoot && sameType && sameBass)
            {
                int newDuration{j["chords"][jsonIndex]["beats"].template get<int>() + next.beats()};
                j["chords"][jsonIndex]["beats"] = newDuration;
                continue;
            }
        }
        // Just add the chord normally if it wasn't the same
        json jsonNext(next);
        j["chords"].push_back(next);
        ++jsonIndex;
    }
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

    for (auto chordJson : j.at("chords"))
    {
        int beatsRemaining{chordJson.at("beats")};
        while (beatsRemaining)
        {
            json copy(chordJson);
            // Use at most 16 beats at a time
            int beats{std::min(16, beatsRemaining)};
            copy["beats"] = beats;
            section.chords.push_back(copy.template get<Chord>());
            beatsRemaining -= beats;
        }
    }
}
