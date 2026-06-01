#pragma once

#include "component_location.h"

struct PhaseImage
{
    bool active = true;

    int clade;
    Location location;

    void Initialize(size_t set_clade, float set_x, float set_y, float set_x_dest, float set_y_dest);
};
