#include <cstdint>

enum class BeatType {
  half = 0,
  quarter = 1,
  eighth = 2,
  sixteenth = 3,
};

class SectionHeader {
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
    SectionHeader (int bpm, BeatType beatType, int beatCount); 
    static SectionHeader decode(uint16_t encoding);
    uint16_t encode() const;

};