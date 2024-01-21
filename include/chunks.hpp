#include <vector>
#include <ostream>
#include <istream>
#include <string>
#include <cstdint>

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
  static SectionHeader read(uint16_t encoding);
  uint16_t write() const;
};

enum class ChordType
{
  // triads
  maj,
  min,
  dim,
  aug,
  // sus
  sus4,
  sus2,
  // sevenths
  maj7,
  min7,
  dom7,
  dim7,
  min7b5,
  maj7s5,
  aug7,
  minmaj7,
  // sixths
  maj6,
  min6,
  maj69,
  min69,
  // ninths
  maj9,
  min9,
  dom9,
  // extended
  min11,
  maj7s11,
  dom13,
  alt,
  // add9
  majadd9,
  minadd9,
};

class Chord
{
  std::string m_root;
  ChordType m_chordType;
  int m_beats;

  static const inline std::vector<ChordType> basicChordsArray{
      ChordType::maj,
      ChordType::min,
      ChordType::maj7,
      ChordType::min7,
      ChordType::dom7,
  };

public:
  Chord(const std::string &root, ChordType chordType, int beats);
  void write(int currentDefaultDuration, std::ostream &outStream) const;
  static Chord read(int currentDefaultDuration, std::istream &inStream);
};
