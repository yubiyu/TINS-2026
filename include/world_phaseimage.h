#pragma once

#include "component_location.h"

struct PhaseImage
{
    bool active = true;

    int caste;
    Location location;

    void Initialize(float set_x, float set_y, float set_x_dest, float set_y_dest);
};
