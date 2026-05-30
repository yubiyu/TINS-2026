#pragma once

#include <cstddef>

struct MimicData
{
    static void Install();

    static const int SPRITE_WIDTH = 64;
    static const int SPRITE_HEIGHT = 64;

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