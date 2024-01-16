template <std::size_t N>
std::array<std::size_t, N> collect(std::size_t offset, std::array<std::size_t, N> shape);

template <std::size_t N>
std::size_t flatten(std::array<std::size_t, N> coords, std::array<std::size_t, N> shape);

#include "indexTransformations.cpp"