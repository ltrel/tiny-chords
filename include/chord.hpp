#pragma once
#include <ostream>
#include <istream>
#include <array>
#include <string>
#include <optional>
#include "enums.hpp"
#include "note.hpp"

class Chord
{
  Note m_root;
  ChordType m_chordType;
  int m_beats;
  std::optional<Note> m_bass;

  static constexpr int letterStateCount{7};
  static constexpr int accidentalStateCount{3};
  static constexpr int shortChordTypeCount{5};
  static constexpr std::array<std::size_t, 3> shortChordShape{letterStateCount, accidentalStateCount, shortChordTypeCount};
  static constexpr std::array<std::size_t, 2> noteShape{letterStateCount, accidentalStateCount};
  static constexpr int maxShortChordIndex{letterStateCount * accidentalStateCount * shortChordTypeCount - 1};

public:
  Chord(Note root, ChordType chordType, int beats, std::optional<Note> bass = std::nullopt);
  void write(int currentDefaultDuration, std::ostream &outStream) const;
  static Chord read(int currentDefaultDuration, std::istream &inStream);
  std::string print() const;

  Note root() const { return m_root; }
  ChordType chordType() const { return m_chordType; }
  int beats() const { return m_beats; }
  std::optional<Note> bass() const {return m_bass; }
};
