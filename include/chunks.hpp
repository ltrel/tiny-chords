#pragma once
#include <vector>
#include <ostream>
#include <istream>
#include <string>
#include <cstdint>
#include "chordtype.hpp"

enum class BeatType
{
  half = 0,
  quarter = 1,
  eighth = 2,
  sixteenth = 3,
};

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
};

class Chord
{
  std::string m_root;
  ChordType m_chordType;
  int m_beats;

public:
  Chord(const std::string &root, ChordType chordType, int beats);
  void write(int currentDefaultDuration, std::ostream &outStream) const;
  static Chord read(int currentDefaultDuration, std::istream &inStream);
};
