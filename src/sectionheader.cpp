#include <stdexcept>
#include <cstdint>
#include <algorithm>
#include <ostream>
#include <istream>
#include "sectionheader.hpp"

void SectionHeader::verifyBeatCount(int beatCount)
{
  if (beatCount < 1 || beatCount > 16)
    throw std::invalid_argument{"beats must be in the range [1,16]"};
}

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
  verifyBeatCount(beatCount);
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

void SectionHeader::setApproxBpm(int bpm)
{
  m_approxBpm = roundTempo(bpm);
}

void SectionHeader::setBeatCount(int beatCount)
{
  verifyBeatCount(beatCount);
  m_beatCount = beatCount;
}