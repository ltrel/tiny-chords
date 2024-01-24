#include <stdexcept>
#include <string_view>
#include <algorithm>
#include <array>
#include <utility>
#include <cstddef>
#include "enums.hpp"

template <typename Left, typename Right, std::size_t N>
struct BiMap
{
  std::array<std::pair<Left, Right>, N> pairs;

  constexpr Right byLeft(const Left &key) const
  {
    const auto it = std::find_if(pairs.begin(), pairs.end(), [&key](const auto &v)
                                  { return v.first == key; });
    if (it != pairs.end()) return it->second;
    throw std::range_error("Not Found");
  }
  constexpr Left byRight(const Right &key) const
  {
    const auto it = std::find_if(pairs.begin(), pairs.end(), [&key](const auto &v)
                                  { return v.second == key; });
    if (it != pairs.end()) return it->first;
    throw std::range_error("Not Found");
  }
};

namespace ChordTypeUtils
{
  std::string_view render(ChordType chordType)
  {
    static constexpr BiMap<ChordType, std::string_view, 5> map{{{
        {ChordType::maj, ""},
        {ChordType::min, "m"},
        {ChordType::maj7, "maj7"},
        {ChordType::min7, "m7"},
        {ChordType::dom7, "7"},
    }}};
    return map.byLeft(chordType);
  }

  static constexpr BiMap<ChordType, std::string_view, 5> serializationMap{{{
      {ChordType::maj, "maj"},
      {ChordType::min, "min"},
      {ChordType::maj7, "maj7"},
      {ChordType::min7, "min7"},
      {ChordType::dom7, "dom7"},
  }}};
  std::string_view serialize(ChordType chordType)
  {
    return serializationMap.byLeft(chordType);
  }
  ChordType deserialize(std::string_view str)
  {
    return serializationMap.byRight(str);
  }
}

namespace BeatTypeUtils
{
  static constexpr BiMap<BeatType, std::string_view, 4> serializationMap{{{
    {BeatType::half, "half"},
    {BeatType::quarter, "quarter"},
    {BeatType::eighth, "eighth"},
    {BeatType::sixteenth, "sixteenth"},
  }}};
  std::string_view serialize(BeatType beatType)
  {
    return serializationMap.byLeft(beatType);
  }
  BeatType deserialize(std::string_view str)
  {
    return serializationMap.byRight(str);
  }
}