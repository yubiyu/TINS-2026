#pragma once

#include <cstddef>

struct Mimic
{
    int caste {};

    bool isCaptured {};
    int detonationCD {};
    bool isExploding {};

    bool inPhasing = true;
    int phasingTicks = 0;

    int health = 1;
    int ticksToAttack = 60 * 4;

    bool isRedirector = false;
    size_t redirectionIndex  = 0;

    bool isEvasive = false;

    bool isStunner = false;

    float xPosition {}; // Centered coords.
    float yPosition {};

    bool inFrameB {};
    int frameChangeTicks_needed = 60 * 0.5;
    int frameChangeTicks_current {};

    int pupilShape {};

    int pupilChangeTicks_needed = 60 * 1.0;
    int pupilChangeTicks_current {};
    float pupilXDisplacement {}, pupilYDisplacement {};

    void Initialize(int set_caste);

    void Update();

    void Render();
    void DisplacePupil();
};