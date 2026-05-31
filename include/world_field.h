#pragma once

#include "component_point.h"

#include <allegro5/allegro5.h>

#include <string>
#include <array>
#include <cstddef>

struct Field
{
    float contamination {};
    float contaminationDoT {};

    float contaminationPerLeak {}; 
    float contaminationDoTPerLeak {};

    float contaminationPerMisplay {};

    float contaminationCleanupRate {};
    float contaminationDoTAttenuation {};

    bool isStunned {};
    int stunRecovery_current {};
    int stunRecovery_Max {};

    int gridWidth{}, gridHeight{};
    int gridXPosition{}, gridYPosition{};

    int gridFrameWidth{}, gridFrameHeight{};
    int gridFrameXPosition{}, gridFrameYPosition{};

    int revertButtonWidth {}, revertButtonHeight {};
    int revertButtonXPosition{}, revertButtonYPosition {};
    int revertButtonFrameXPosition{}, revertButtonFrameYPosition{};

    int tachyonBarMaxWidth {}, tachyonBarHeight {};
    float tachyonBarCurrentWidth {};
    int tachyonBarXPosition{}, tachyonBarYPosition{};

    float tachyonBarPhaseShift_Current {};
    float tachyonBarPhaseShift_Max {};
    float tachyonBarPhaseShift_Change {};

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
    static constexpr int indexToKeypad[9]{
        7, // Grid index [0] can be accessed by pressing keypad 7.
        8,
        9,
        4,
        5,
        6,
        1,
        2,
        3
    };

    int attackNumTicks {};
    int attackCD_Required {};
    int attackCD_current {};

    bool cellUnderAttack[GRID_CELLS] {};
    int cellAttackProgress[GRID_CELLS] {};

    int baselineSpawnCD{};
    int spawnCDUpperLimit{};
    int spawnCDLowerLimit{};
    int currentSpawnCD{};

    size_t capturerFrame[GRID_CELLS] {};

    Point2Df cellXYPosition[GRID_CELLS] {};


    // Centered coords.
    Point2D dialogFrameXY {};
    Point2D dialogErrorXY {};
    Point2D dialogGravimetricInterferenceXY {};
    Point2D dialogRecalibratingXY {};


    int dialogDisplacementTicks_current {};
    int dialogDisplacementTicks_max {};
    Point2D dialogFrameDisplacement {};
    Point2D dialogErrorDisplacement {};
    Point2D dialogGravimetricInterferenceDisplacement {};
    Point2D dialogRecalibratingDisplacement {};

    static Field field;
    void Initialize();
    void Reset();

    void Update();
    void ResetSpawnCD();
    void ProgressSpawnCD();
    int SimultaneousSpawnRNG();

    void UpdateContaminationBar();

    void Stun();
    void DisplaceStunDialog();
};