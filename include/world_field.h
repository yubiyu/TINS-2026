#pragma once

#include "world_mimic.h"
#include "data_mimic.h"

#include <allegro5/allegro5.h>

#include <string>
#include <array>
#include <cstddef>

struct Field
{
    int gridWidth {}, gridHeight {};
    int gridXPosition {}, gridYPosition {};

    int frameWidth {}, frameHeight {};
    int frameXPosition {}, frameYPosition {};

    std::string titleString {};
    int titleStringXPosition {}, titleStringYPosition {};

    static const size_t GRID_COLS = 3;
    static const size_t GRID_ROWS = 3;
    static const size_t GRID_CELLS = GRID_COLS * GRID_ROWS;

    std::array<Mimic*, GRID_CELLS> gridMimics {};

    std::array<int, MimicData::NUM_CASTES> mimicsCaptured {};

    static constexpr int keypadToIndex[10] =
        {
            -1, // 0 unused
            6,  // Looks up grid index [6] when keypad 1 is pressed.
            7,  // 2
            8,  // 3
            3,  // 4
            4,  // 5
            5,  // 6
            0,  // 7
            1,  // 8
            2   // 9
    };

    static Field field;

    void Initialize();
    void Reset();
};