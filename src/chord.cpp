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

Chord::Chord(const std::string &root, ChordType chordType, int beats, const std::string &bass)
    : m_root{root}, m_chordType{chordType}, m_beats{beats}, m_bass{bass}
{
  if (beats < 1 || beats > 16)
    throw std::invalid_argument{"beats must be in the range [1,16]"};

  if (!isNoteStringValid(root))
    throw std::invalid_argument{"root must be a valid note with at most a single accidental"};
  m_root[0] = std::toupper(m_root[0], std::locale());

  if (!bass.empty())
  {
    if (isNoteStringValid(bass))
    {
      m_bass[0] = std::toupper(m_bass[0], std::locale());
      // No need to store the bass note separately if it is the root
      if (m_bass == m_root) m_bass = "";
    }
    else throw std::invalid_argument{"bass must be a valid note with at most a single accidental"};
  }
}

Chord Chord::read(int currentDefaultDuration, std::istream &inStream)
{
  uint8_t current{0};
  inStream.read(reinterpret_cast<char*>(&current), 1);

  std::string bass{};
  // Check for slash chord prefix
  if ((current & 1) == 0 && current > (maxShortChordIndex << 1))
  {
    std::size_t slashChordIndex{static_cast<std::size_t>((current >> 1) - (maxShortChordIndex))};
    std::array<std::size_t, 2> indices{collect<2>(slashChordIndex, {letterStateCount, accidentalStateCount})};
    bass = indices[0] + 'A';
    std::size_t accidentalIndex{indices[1]};
    if (accidentalIndex == 1) bass.append({'b'});
    else if (accidentalIndex == 2) bass.append({'#'});
    // Advance the input stream to the body of the chord
    inStream.read(reinterpret_cast<char*>(&current), 1);
  }

  // LSb not set means short form chord
  if ((current & 1) == 0)
  {
    std::size_t combinedIndex{static_cast<std::size_t>(current >> 1)};
    std::array<size_t, 3> indices{collect<3>(combinedIndex, shortChordShape)};
    std::string root{static_cast<char>(indices[0] + 'A')};
    std::size_t accidentalIndex{indices[1]};
    ChordType chordType{static_cast<ChordType>(indices[2])};
    if (accidentalIndex == 1) root.append({'b'});
    else if (accidentalIndex == 2) root.append({'#'});
    return {root, chordType, currentDefaultDuration, bass};
  }
  // Long form chords not implemented yet
  return {"A", ChordType::maj, 4};
}

void Chord::write(int currentDefaultDuration, std::ostream &outStream) const
{
  if (!m_bass.empty())
  {
    std::size_t rootIndex{static_cast<std::size_t>((m_bass[0] - 'A'))};
    std::size_t accidentalIndex{0};
    if (m_bass.length() == 2)
      accidentalIndex = (m_bass[1] == 'b') ? 1 : 2;
    std::size_t slashChordIndex{flatten<2>({rootIndex, accidentalIndex}, noteShape)};
    auto encoding{(maxShortChordIndex + slashChordIndex) << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }

  std::size_t chordTypeIndex{static_cast<std::size_t>(m_chordType)};
  if (currentDefaultDuration == m_beats && chordTypeIndex < shortChordTypeCount)
  {
    std::size_t rootIndex{static_cast<std::size_t>((m_root[0] - 'A'))};
    std::size_t accidentalIndex{0};
    if (m_root.length() == 2)
      accidentalIndex = (m_root[1] == 'b') ? 1 : 2;
    std::size_t combinedIndex{flatten<3>({rootIndex, accidentalIndex, chordTypeIndex}, shortChordShape)};
    auto encoding{combinedIndex << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }
}

std::string Chord::print() const
{
  if (m_bass.empty())
    return std::format("{}{} : {} beats", m_root, ChordTypeUtils::render(m_chordType), m_beats);
  return std::format("{}{}/{} : {} beats", m_root, ChordTypeUtils::render(m_chordType), m_bass, m_beats);
}