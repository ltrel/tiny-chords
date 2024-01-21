#pragma once
#include <array>
#include <cstddef>

template <std::size_t N>
std::array<std::size_t, N> collect(std::size_t offset, std::array<std::size_t, N> shape)
{
    std::array<std::size_t, N> coords{};
    for (std::size_t i{0}; i < N; i++)
    {
        coords[i] = offset % shape[i];
        offset = offset / shape[i];
    }
    return coords;
}

template <std::size_t N>
std::size_t flatten(std::array<std::size_t, N> coords, std::array<std::size_t, N> shape)
{
    std::size_t offset{0};
    std::size_t factor{1};
    for (std::size_t i{0}; i < N; i++)
    {
        offset += coords[i] * factor;
        factor *= shape[i];
    }
    return offset;
}