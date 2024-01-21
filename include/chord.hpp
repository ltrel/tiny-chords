#pragma once
#include <ostream>
#include <istream>
#include <string>
#include "chordtype.hpp"

class Chord
{
  std::string m_root;
  ChordType m_chordType;
  int m_beats;

public:
  Chord(const std::string &root, ChordType chordType, int beats);
  void write(int currentDefaultDuration, std::ostream &outStream) const;
  static Chord read(int currentDefaultDuration, std::istream &inStream);
  std::string print() const;
};
