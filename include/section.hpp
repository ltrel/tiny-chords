#pragma once
#include <vector>
#include <istream>
#include <ostream>
#include "sectionheader.hpp"
#include "beattype.hpp"
#include "chord.hpp"

struct Section {
    SectionHeader header{120, BeatType::quarter, 4};
    std::vector<Chord> chords{};

    static Section read(std::istream &inStream);
    void write(std::ostream &outStream) const;
};
