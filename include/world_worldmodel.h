#pragma once

#include "world_field.h"
#include "world_mimic.h"
#include "world_phaseimage.h"
#include "world_radiation.h"

#include "data_mimic.h"

class WorldModel
{
public:

    std::array<Mimic*, Field::GRID_CELLS> mimicGrid {};
    std::array<int, MimicData::NUM_CASTES> mimicsCaptured {};

    static WorldModel world; // Shared world instance.

    std::array<size_t, Field::GRID_CELLS>redirectionArray {};

    std::vector<PhaseImage*>phaseImages;
    std::vector<Radiation*>radiation;

    void Initialize();
    void Uninitialize();
    void Reset();

    void Update();
    
    void SpawnMimic();
    void SpawnExplosionRadiation(float origin_x, float origin_y);

    void InitiateAttackCell(size_t cell_index);
    void CompleteAttackCell(size_t cell_index);

    void UpdateRedirectionArray();
    void SetDefaultRedirectionArray();
};