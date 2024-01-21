#include <array>
#include <cstddef>
#include <string>
#include <cstdint>
#include <format>
#include <locale>
#include <ostream>
#include <istream>
#include <stdexcept>
#include "chordtype.hpp"
#include "chord.hpp"
#include "indextransformations.hpp"

bool isNoteStringValid(const std::string &note)
{
  if (note.length() < 1 || note.length() > 2)
    return false;
  char letter{std::toupper(note[0], std::locale())};
  if (letter < 'A' || letter > 'G')
    return false;
  return note.length() == 1 || note[1] == 'b' || note[1] == '#';
}

Chord::Chord(const std::string &root, ChordType chordType, int beats)
    : m_root{root}, m_chordType{chordType}, m_beats{beats}
{
  if (beats < 1 || beats > 16)
    throw std::invalid_argument{"beats must be in the range [1,16]"};
  if (!isNoteStringValid(root))
    throw std::invalid_argument{"root must be a valid note with at most a single accidental"};

  m_root[0] = std::toupper(m_root[0], std::locale());
}

Chord Chord::read(int currentDefaultDuration, std::istream &inStream)
{
  uint8_t first{0};
  inStream.read(reinterpret_cast<char*>(&first), 1);
  if ((first & 1) == 0)
  {
    std::size_t combinedIndex{static_cast<std::size_t>(first >> 1)};
    std::array<size_t, 3> indices{collect<3>(combinedIndex, {7, 3, 5})};
    std::string root{static_cast<char>(indices[0] + 'A')};
    std::size_t accidentalIndex{indices[1]};
    ChordType chordType{static_cast<ChordType>(indices[2])};
    if (accidentalIndex == 1) root.append({'b'});
    else if (accidentalIndex == 2) root.append({'#'});
    return {root, chordType, currentDefaultDuration};
  }
  return {"A", ChordType::maj, 4};
}

void Chord::write(int currentDefaultDuration, std::ostream &outStream) const
{
  std::size_t chordTypeIndex{static_cast<std::size_t>(m_chordType)};
  if (currentDefaultDuration == m_beats && chordTypeIndex <= 4)
  {
    std::size_t rootIndex{static_cast<std::size_t>((m_root[0] - 'A') * 3)};
    std::size_t accidentalIndex{0};
    if (m_root.length() == 2)
      accidentalIndex = (m_root[1] == 'b') ? 1 : 2;
    std::size_t combinedIndex{flatten<3>({rootIndex, accidentalIndex, chordTypeIndex}, {7, 3, 5})};
    auto encoding{combinedIndex << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }
}

std::string Chord::print() const
{
  return std::format("{}{} : {} beats", m_root, renderChordType(m_chordType), m_beats);
}