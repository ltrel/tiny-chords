#pragma once
#include <string_view>
#include <stdexcept>
#include <array>
#include <algorithm>
#include <utility>

enum class BeatType
{
  half = 0,
  quarter = 1,
  eighth = 2,
  sixteenth = 3,
};

enum class ChordType
{
  // short form
  maj,
  min,
  maj7,
  min7,
  dom7,
  // other triads
  dim,
  aug,
  // other sevenths
  dim7,
  min7b5,
  maj7s5,
  aug7,
  minmaj7,
  // sus
  sus4,
  sus2,
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

namespace ChordTypeUtils
{
  std::string_view render(ChordType chordType);
  std::string_view serialize(ChordType chordType);
  ChordType deserialize(std::string_view str);
}

namespace BeatTypeUtils
{
  std::string_view serialize(BeatType beatType);
  BeatType deserialize(std::string_view str);
}