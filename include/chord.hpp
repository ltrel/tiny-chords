#pragma once
#include <ostream>
#include <istream>
#include <array>
#include <string>
#include "chordtype.hpp"

class Chord
{
  std::string m_root;
  ChordType m_chordType;
  int m_beats;
  std::string m_bass;

  static constexpr int letterStateCount{7};
  static constexpr int accidentalStateCount{3};
  static constexpr int shortChordTypeCount{5};
  static constexpr std::array<std::size_t, 3> shortChordShape{letterStateCount, accidentalStateCount, shortChordTypeCount};
  static constexpr std::array<std::size_t, 2> noteShape{letterStateCount, accidentalStateCount};
  static constexpr int maxShortChordIndex{letterStateCount * accidentalStateCount * shortChordTypeCount - 1};

public:
  Chord(const std::string &root, ChordType chordType, int beats, const std::string &bass = "");
  void write(int currentDefaultDuration, std::ostream &outStream) const;
  static Chord read(int currentDefaultDuration, std::istream &inStream);
  std::string print() const;
};
