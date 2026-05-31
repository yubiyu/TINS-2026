#pragma once

#include <array>
#include <cstddef>

#include "component_point.h"

struct MimicData
{
    static void Install();

    enum enumMimicCaste : size_t
    {
        CASTE_MOOK,
        CASTE_CRAB,
        CASTE_REDIRECTOR,
        CASTE_EVADER,
        CASTE_STUNNER,
        CASTE_SPLITTER,
        CASTE_BOMB,
        CASTE_CASTELLAN
    };
    static const size_t NUM_CASTES = CASTE_CASTELLAN + 1;

    enum enumPupilShapes : size_t
    {
        PUPIL_DOT,
        PUPIL_1,
        PUPIL_2,
        PUPIL_3,
        PUPIL_4,
        PUPIL_5,
        PUPIL_6,
        PUPIL_7,
        PUPIL_8,
        PUPIL_9
    };
    static const size_t NUM_PUPIL_SHAPES = PUPIL_9 + 1;
    static const size_t NUM_PUPIL_VARIANTS = 4;


    static const int SPRITE_WIDTH = 64;
    static const int SPRITE_HEIGHT = 64;

    static int PHASING_DISTANCE;
    static int PHASING_TIME;
    static int PHASING_SPEED; 

    static int TICKS_TO_DETONATION;
};