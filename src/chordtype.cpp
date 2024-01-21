#include <string_view>
#include "chordtype.hpp"

std::string_view renderChordType(ChordType chordType)
{
  switch (chordType)
  {
  case ChordType::maj:
    return "";
  case ChordType::min:
    return "m";
  case ChordType::maj7:
    return "maj7";
  case ChordType::min7:
    return "m7";
  case ChordType::dom7:
    return "7";
  default:
    return "unknown";
  }
}