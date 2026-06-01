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

    bool gameFailed {};
    int gameProgress {};
    bool usingMaxSpawnableClade {};

    std::array<Mimic*, Field::GRID_CELLS> mimicGrid {};
    std::array<int, MimicData::NUM_CLADES> mimicsCaptured {};

    std::array<size_t, Field::GRID_CELLS>redirectionArray {};

    std::vector<PhaseImage*>phaseImages;
    std::vector<Radiation*>radiations;
    std::vector<StunLightning*>stunLightnings;




    std::vector<Mimic*> looseMimics {};
    std::array<int, MimicData::NUM_CLADES> chaosScore {};
    int chaosScoreTicks_current{};
    int chaosScoreTicks_Max = 4;

    static WorldModel world; // Shared world instance.
    void Initialize();
    void Uninitialize();
    void Reset();

    void Update();
    
    void SpawnMimicToGrid();
    void SpawnMimicBreached();
    void SpawnMimic_Splitters(int origin_col, int origin_row);
    void SpawnPhaseImages(size_t set_clade, float set_x, float set_y, bool divergent);
    void SpawnExplosionRadiation(float origin_x, float origin_y);
    void SpawnStunLightning(float origin_x, float origin_y);

    void InitiateAttackCell(size_t cell_index);
    void CompleteAttackCell(size_t cell_index);
    void AddCapture(size_t which_clade);
    void Misplay();
    void Leak();

    void UpdateRedirectionArray();
    void SetDefaultRedirectionArray();
};