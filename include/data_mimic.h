#pragma once

#include <cstddef>

struct MimicData
{
    static void Install();

    static const int SPRITE_WIDTH = 64;
    static const int SPRITE_HEIGHT = 64;

    static int PHASING_DISTANCE;
    static int PHASING_TIME;
    static int PHASING_SPEED; 

    enum enumMimicCaste
    {
        CASTE_DRONE,
        CASTE_ARMOURED,
        CASTE_REDIRECTOR,
        CASTE_EVADER,
        CASTE_STUNNER,
        CASTE_SPLITTER,
        CASTE_BOMB,
        CASTE_CASTELLAN
    };
    static const size_t NUM_CASTES = CASTE_CASTELLAN;

};