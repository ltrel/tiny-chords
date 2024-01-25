#include "note.hpp"
#include <string_view>
#include <stdexcept>
#include <string>

int parseLetter(std::string_view str)
{
  char val{str.at(0) - 'A'};
  if (val < 0 || val > 6)
    throw std::invalid_argument{"First character must be [A-G]"};

  return val;
}

int parseAccidental(std::string_view str)
{
  if (str.length() > 2)
    throw std::invalid_argument{"Note should be at most two characters"};
  if (str.length() == 1)
    return 0;
  switch (str.at(1))
  {
  case 'b':
    return 1;
  case '#':
    return 2;
  default:
    throw std::invalid_argument{"Invalid accidental character"};
  }
}

Note::Note(std::string_view str)
    : m_letterIndex{parseLetter(str)}, m_accidentalIndex{parseAccidental(str)} {}

Note::Note(int letterIndex, int accidentalIndex)
    : m_letterIndex(letterIndex), m_accidentalIndex(accidentalIndex)
{
  if (letterIndex < 0 || letterIndex > 6)
    throw std::invalid_argument{"Invalid letterIndex"};
  if (m_accidentalIndex < 0 || m_accidentalIndex > 2)
    throw std::invalid_argument{"Invalid accidentalIndex"};
}

std::string Note::print() const
{
  std::string ret{static_cast<char>(m_letterIndex + 'A')};
  if (m_accidentalIndex == 1) ret += 'b';
  else if (m_accidentalIndex == 2) ret += '#';
  return ret;
}