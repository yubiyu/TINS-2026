#pragma once

#include "world_field.h"
#include "world_mimic.h"
#include "world_phaseimage.h"

#include "data_mimic.h"

class WorldModel
{
public:

    std::array<Mimic*, Field::GRID_CELLS> mimicGrid {};
    std::array<int, MimicData::NUM_CASTES> mimicsCaptured {};

    static WorldModel world; // Shared world instance.

    std::vector<PhaseImage*>phaseImages;

    void Initialize();
    void Uninitialize();
    void Reset();

    void Update();
    
    void SpawnMimic();
    void InitiateAttackCell(size_t cell_index);
    void CompleteAttackCell(size_t cell_index);
};