#include <algorithm>
#include <vector>
#include <array>
#include <cstddef>
#include <iterator>
#include <string>
#include <cstdint>
#include <locale>
#include <ostream>
#include <istream>
#include <stdexcept>
#include "chordtype.hpp"
#include "chunks.hpp"
#include "indextransformations.hpp"

int SectionHeader::roundTempo(int bpm)
{
  int below{(bpm / tempoIncrement) * tempoIncrement};
  int above{below + tempoIncrement};
  int closest{(above - bpm < bpm - below) ? above : below};
  return std::clamp<int>(closest, minTempo, maxTempo);
}

SectionHeader::SectionHeader(int bpm, BeatType beatType, int beatCount)
    : m_approxBpm{roundTempo(bpm)}, m_beatType{beatType}, m_beatCount{beatCount}
{
  if (m_beatCount - 1 > beatCountMask || m_beatCount < 1)
  {
    throw std::invalid_argument{"beatCount - 1  must fit in a 4-bit unsigned integer"};
  }
};

void SectionHeader::write(std::ostream &outStream) const
{
  uint16_t encoded{0};
  encoded |= (m_approxBpm - minTempo) / tempoIncrement;
  encoded |= static_cast<int>(m_beatType) << beatTypeShift;
  encoded |= (m_beatCount - 1) << beatCountShift;
  outStream.write(reinterpret_cast<const char*>(&encoded), sizeof(uint16_t));
}

SectionHeader SectionHeader::read(std::istream &inStream)
{
  uint16_t encoding{0};
  inStream.read(reinterpret_cast<char*>(&encoding), sizeof(uint16_t));
  int tempo{(encoding & tempoMask) * tempoIncrement + minTempo};
  BeatType beatType{static_cast<BeatType>(encoding >> beatTypeShift & beatTypeMask)};
  int beatCount{(encoding >> beatCountShift & beatCountMask) + 1};

  return SectionHeader{tempo, beatType, beatCount};
}

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
    std::array<size_t, 2> indices{collect<2>(combinedIndex, {7 * 3, 5})};
    ChordType chordType{static_cast<ChordType>(indices[1])};
    std::string root{static_cast<char>(indices[0] / 3 + 'A')};
    std::size_t accidentalIndex{indices[0] % 3};
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
    if (m_root.length() == 2)
    {
      rootIndex += (m_root[1] == 'b') ? 1 : 2;
    }
    std::size_t combinedIndex{flatten<2>({rootIndex, chordTypeIndex}, {7 * 3, 5})};
    auto encoding{combinedIndex << 1};
    outStream.write(reinterpret_cast<const char*>(&encoding), 1);
  }
}

