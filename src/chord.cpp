#include <array>
#include <cstddef>
#include <string>
#include <cstdint>
#include <format>
#include <locale>
#include <ostream>
#include <istream>
#include <stdexcept>
#include "enums.hpp"
#include "chord.hpp"
#include "note.hpp"
#include "indextransformations.hpp"

Chord::Chord(Note root, ChordType chordType, int beats, std::optional<Note> bass)
    : m_root{root}, m_chordType{chordType}, m_beats{beats}, m_bass{bass}
{
  if (beats < 1 || beats > 16)
    throw std::invalid_argument{"beats must be in the range [1,16]"};
}

Chord Chord::read(int currentDefaultDuration, std::istream &inStream)
{
  uint8_t current{0};
  inStream.read(reinterpret_cast<char*>(&current), 1);

  std::optional<Note> bass{std::nullopt};
  // Check for slash chord prefix
  if ((current & 1) == 0 && current > (maxShortChordIndex << 1))
  {
    int slashChordIndex{(current >> 1) - (maxShortChordIndex)};
    std::array<int, 2> indices{collect<2>(slashChordIndex, {letterStateCount, accidentalStateCount})};
    bass = {indices[0], indices[1]};
    // Advance the input stream to the body of the chord
    inStream.read(reinterpret_cast<char*>(&current), 1);
  }

  // LSb not set means short form chord
  if ((current & 1) == 0)
  {
    int combinedIndex{current >> 1};
    std::array<int, 3> indices{collect<3>(combinedIndex, shortChordShape)};
    Note root{indices[0], indices[1]};
    ChordType chordType{static_cast<ChordType>(indices[2])};
    return {root, chordType, currentDefaultDuration, bass};
  }
  // Long form chords not implemented yet
  return {Note("A"), ChordType::maj, 4};
}

void Chord::write(int currentDefaultDuration, std::ostream &outStream) const
{
  if (m_bass.has_value())
  {

    int rootIndex{m_bass->letterIndex()};
    int accidentalIndex{m_bass->accidentalIndex()};
    int slashChordIndex{flatten<2>({rootIndex, accidentalIndex}, noteShape)};
    auto encoding{(maxShortChordIndex + slashChordIndex) << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }

  int chordTypeIndex{static_cast<int>(m_chordType)};
  if (currentDefaultDuration == m_beats && chordTypeIndex < shortChordTypeCount)
  {
    int rootIndex{m_root.letterIndex()};
    int accidentalIndex{m_root.accidentalIndex()};
    int combinedIndex{flatten<3>({rootIndex, accidentalIndex, chordTypeIndex}, shortChordShape)};
    auto encoding{combinedIndex << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }
}

std::string Chord::print() const
{
  if (m_bass.has_value())
    return std::format("{}{}/{} : {} beats", m_root.print(), ChordTypeUtils::render(m_chordType), m_bass->print(), m_beats);
  return std::format("{}{} : {} beats", m_root.print(), ChordTypeUtils::render(m_chordType), m_beats);
}