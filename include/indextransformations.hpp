#pragma once
#include <array>
#include <cstddef>

template <std::size_t N>
std::array<int, N> collect(int offset, std::array<int, N> shape)
{
    std::array<int, N> coords{};
    for (std::size_t i{0}; i < N; i++)
    {
        coords[i] = offset % shape[i];
        offset = offset / shape[i];
    }
    return coords;
}

template <std::size_t N>
int flatten(std::array<int, N> coords, std::array<int, N> shape)
{
    int offset{0};
    int factor{1};
    for (std::size_t i{0}; i < N; i++)
    {
        offset += coords[i] * factor;
        factor *= shape[i];
    }
    return offset;
}