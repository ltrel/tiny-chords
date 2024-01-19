#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "chunks.hpp"

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

uint16_t SectionHeader::encode() const
{
  uint16_t encoded{0};
  encoded |= (m_approxBpm - minTempo) / tempoIncrement;
  encoded |= static_cast<int>(m_beatType) << beatTypeShift;
  encoded |= (m_beatCount - 1) << beatCountShift;
  return encoded;
}

SectionHeader SectionHeader::decode(uint16_t encoding)
{
  int tempo{(encoding & tempoMask) * tempoIncrement + minTempo};
  BeatType beatType{static_cast<BeatType>(encoding >> beatTypeShift & beatTypeMask)};
  int beatCount{(encoding >> beatCountShift & beatCountMask) + 1};

  return SectionHeader{tempo, beatType, beatCount};
}