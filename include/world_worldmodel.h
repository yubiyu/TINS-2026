#pragma once

#include "world_field.h"
#include "world_mimic.h"
#include "world_phaseimage.h"
#include "world_radiation.h"
#include "world_stunlightning.h"

#include "data_mimic.h"

class WorldModel
{
public:

    int gameProgress {};
    bool usingMaxSpawnableClade {};

    std::array<Mimic*, Field::GRID_CELLS> mimicGrid {};
    std::array<int, MimicData::NUM_CLADES> mimicsCaptured {};

    std::array<size_t, Field::GRID_CELLS>redirectionArray {};

    std::vector<PhaseImage*>phaseImages;
    std::vector<Radiation*>radiations;
    std::vector<StunLightning*>stunLightnings;


    static WorldModel world; // Shared world instance.

    void Initialize();
    void Uninitialize();
    void Reset();

    void Update();
    
    void SpawnMimic();
    void SpawnMimic_Splitters(int origin_col, int origin_row);
    void SpawnExplosionRadiation(float origin_x, float origin_y);
    void SpawnStunLightning(float origin_x, float origin_y);

    void InitiateAttackCell(size_t cell_index);
    void CompleteAttackCell(size_t cell_index);
    void AddCapture(size_t which_clade);
    void Misplay();

    void UpdateRedirectionArray();
    void SetDefaultRedirectionArray();
};