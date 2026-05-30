#pragma once

#include <cstddef>

struct Mimic
{
    int caste {};

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

    void Initialize(int set_caste);

    void Render();
};