#pragma once

#include "data_direction.h"

#include "world_field.h"
#include "world_mimic.h"

class WorldModel
{

public:
    static WorldModel world; // Shared world instance.

    void Initialize();
    void Uninitialize();

    void Update();
};