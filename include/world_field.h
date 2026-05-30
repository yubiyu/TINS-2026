#pragma once

#include <allegro5/allegro5.h>

#include <string>
#include <array>
#include <cstddef>

struct Field
{
    int gridWidth{}, gridHeight{};
    int gridXPosition{}, gridYPosition{};

    int frameWidth{}, frameHeight{};
    int frameXPosition{}, frameYPosition{};

    std::string titleString{};
    int titleStringXPosition{}, titleStringYPosition{};

    static const size_t GRID_COLS = 3;
    static const size_t GRID_ROWS = 3;
    static const size_t GRID_CELLS = GRID_COLS * GRID_ROWS;

    static constexpr int keypadToIndex[9]{
        6, // Looks up grid index [6] when keypad 1 is pressed.
        7, // 2
        8, // 3
        3, // 4
        4, // 5
        5, // 6
        0, // 7
        1, // 8
        2  // 9
    };

    int attackNumTicks {};

    bool cellUnderAttack[GRID_CELLS] {};
    int cellAttackProgress[GRID_CELLS] {};

    int baselineSpawnCD{};
    int spawnCDUpperLimit{};
    int spawnCDLowerLimit{};
    int currentSpawnCD{};

    static Field field;

    void Initialize();
    void Reset();

    void ResetSpawnCD();
    void ProgressSpawnCD();
    int SimultaneousSpawnRNG();
};