#include <vector>
#include <istream>
#include <utility>
#include <ostream>
#include "section.hpp"
#include "chord.hpp"

Section Section::read(std::istream &inStream)
{
    SectionHeader header{SectionHeader::read(inStream)};
    Section section{header, {}};

    while (inStream.peek() != EOF)
    {
        section.chords.push_back(Chord::read(section.header.beatCount(), inStream));
    }

    return section;
}

void Section::write(std::ostream &outStream) const
{
    header.write(outStream);
    for (const auto &chord : chords)
    {
        chord.write(header.beatCount(), outStream);
    }
}
