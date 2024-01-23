#pragma once
#include <istream>
#include <ostream>
#include "beattype.hpp"

class SectionHeader
{
  int m_approxBpm;
  BeatType m_beatType;
  int m_beatCount;

  static constexpr int tempoBitSize{6};
  static constexpr int beatTypeBitSize{2};
  static constexpr int beatCountBitSize{4};

  static constexpr int tempoMask{(1 << tempoBitSize) - 1};
  static constexpr int beatTypeMask{(1 << beatTypeBitSize) - 1};
  static constexpr int beatCountMask{(1 << beatCountBitSize) - 1};

  static constexpr int beatTypeShift{tempoBitSize};
  static constexpr int beatCountShift{tempoBitSize + beatTypeBitSize};

  static constexpr int tempoIncrement{4};
  static constexpr int minTempo{40};
  static constexpr int maxTempo{minTempo + tempoIncrement * tempoMask};

  static int roundTempo(int bpm);

public:
  SectionHeader(int bpm, BeatType beatType, int beatCount);
  static SectionHeader read(std::istream &inStream);
  void write(std::ostream &outStream) const;

  int approxBpm() const { return m_approxBpm; }
  BeatType beatType() const { return m_beatType; }
  int beatCount() const { return m_beatCount; }
};
