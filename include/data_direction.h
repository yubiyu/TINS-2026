#pragma once

#include <cstdint>

struct Direction
{
    static const bool DIAGONALS_PERMITTED = true;
    static const int NO_DIRECTION = -1; // Not counted as one of four directions. Definitely should not be used as an array index.

    enum enumDirections
    {
        EAST = 0,
        NORTHEAST = 1,
        NORTH = 2,
        NORTHWEST = 3,
        WEST = 4,
        SOUTHWEST = 5,
        SOUTH = 6,
        SOUTHEAST = 7
    };
    static const size_t DIRECTIONS_MARKER_BEGIN = EAST;
    static const size_t DIRECTIONS_MARKER_END = SOUTHEAST;
    static const size_t NUM_DIRECTIONS = DIRECTIONS_MARKER_END + 1;
    static const size_t NUM_ORTHOGONAL_DIRECTIONS = NUM_DIRECTIONS / 2;
    enum enumDirectionBits : uint8_t
    {
        EAST_BIT = 1 << EAST,           // 00000001
        NORTHEAST_BIT = 1 << NORTHEAST, // 00000010
        NORTH_BIT = 1 << NORTH,         // 00000100
        NORTHWEST_BIT = 1 << NORTHWEST, // 00001000
        WEST_BIT = 1 << WEST,           // 00010000
        SOUTHWEST_BIT = 1 << SOUTHWEST, // 00100000
        SOUTH_BIT = 1 << SOUTH,         // 01000000
        SOUTHEAST_BIT = 1 << SOUTHEAST  // 10000000
    };

    using direction = int; // Don't change this to unsigned because NO_DIRECTION is negative.
    static direction DirectionFromVector(int dx, int dy);
};