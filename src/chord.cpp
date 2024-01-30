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
#include "sectionheader.hpp"
#include "note.hpp"
#include "indextransformations.hpp"

Chord::Chord(Note root, ChordType chordType, int beats, std::optional<Note> bass)
    : m_root{root}, m_chordType{chordType}, m_beats{beats}, m_bass{bass}
{
  SectionHeader::verifyBeatCount(beats);
}

Chord::Chord() : m_root{Note("A")}, m_chordType{ChordType::maj}, m_beats{4}, m_bass{std::nullopt} {}

Chord Chord::read(int currentDefaultDuration, std::istream &inStream)
{
  uint8_t first{0};
  inStream.read(reinterpret_cast<char*>(&first), 1);

  std::optional<Note> bass{std::nullopt};
  // Check for slash chord prefix
  if ((first & 1) == 0 && first > (maxShortChordIndex << 1))
  {
    int slashChordIndex{(first >> 1) - (maxShortChordIndex)};
    std::array<int, 2> indices{collect<2>(slashChordIndex, {letterStateCount, accidentalStateCount})};
    bass = {indices[0], indices[1]};
    // Advance the input stream to the body of the chord
    inStream.read(reinterpret_cast<char*>(&first), 1);
  }

  // LSb not set means short form chord
  if ((first & 1) == 0)
  {
    int combinedIndex{first >> 1};
    std::array<int, 3> indices{collect<3>(combinedIndex, shortChordShape)};
    Note root{indices[0], indices[1]};
    ChordType chordType{static_cast<ChordType>(indices[2])};
    return {root, chordType, currentDefaultDuration, bass};
  }
  else
  {
    uint8_t second{0};
    inStream.read(reinterpret_cast<char*>(&second), 1);
    int full{first + (second << 8)};
    int combinedIndex{full >> 1};
    std::array<int, 4> indices{collect<4>(combinedIndex, longChordShape)};
    Note root{indices[0], indices[1]};
    ChordType chordType{static_cast<ChordType>(indices[2])};
    // Reverse the range shift performed during write
    int beats{indices[3] + 1};
    return {root, chordType, beats, bass};
  }
}

void Chord::write(int currentDefaultDuration, std::ostream &outStream) const
{
  if (m_bass.has_value())
  {

    int letterIndex{m_bass->letterIndex()};
    int accidentalIndex{m_bass->accidentalIndex()};
    int slashChordIndex{flatten<2>({letterIndex, accidentalIndex}, noteShape)};
    auto encoding{(maxShortChordIndex + slashChordIndex) << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }

  int letterIndex{m_root.letterIndex()};
  int accidentalIndex{m_root.accidentalIndex()};
  int chordTypeIndex{static_cast<int>(m_chordType)};
  int encoding{};
  int byteCount{};
  if (currentDefaultDuration == m_beats && chordTypeIndex < shortChordTypeCount)
  {
    int combinedIndex{flatten<3>({letterIndex, accidentalIndex, chordTypeIndex}, shortChordShape)};
    encoding = combinedIndex << 1;
    byteCount = 1;
  }
  else
  {
    // Subtract one from beats since zero is not a valid duration
    int combinedIndex{flatten<4>({letterIndex, accidentalIndex, chordTypeIndex, m_beats - 1}, longChordShape)};
    // Set the LSb so the reader knows the next byte is needed
    encoding = combinedIndex << 1 | 1;
    byteCount = 2;
  }
  outStream.write(reinterpret_cast<const char*>(&encoding), byteCount);
}

std::string Chord::print() const
{
  if (m_bass.has_value())
    return std::format("{}{}/{} : {} beats", m_root.print(), ChordTypeUtils::render(m_chordType), m_bass->print(), m_beats);
  return std::format("{}{} : {} beats", m_root.print(), ChordTypeUtils::render(m_chordType), m_beats);
}

void Chord::setBeats(int beats)
{
  SectionHeader::verifyBeatCount(beats);
  m_beats = beats;
}