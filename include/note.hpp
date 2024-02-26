#pragma once
#include <string_view>
#include <string>

class Note {
  int m_letterIndex;
  int m_accidentalIndex;

public:
  Note(std::string_view str);
  Note(int letterIndex, int accidentalIndex);
  std::string print() const;

  int letterIndex() const { return m_letterIndex; }
  int accidentalIndex() const { return m_accidentalIndex; }
};

bool operator == (const Note &n1, const Note &n2);