#include "world_worldmodel.h"

#include "core_uistate.h"
#include "core_random.h"

#include "ui_worldview.h"

#include "data_field.h"

#include <iostream>
#include <algorithm>

WorldModel WorldModel::world;

void WorldModel::Initialize()
{
    Field::field.Initialize();
}

void WorldModel::Uninitialize()
{
    for(size_t i = 0; i < mimicGrid.size(); i++)
    {
        if(mimicGrid[i])
            delete mimicGrid[i];
        
        mimicGrid[i] = nullptr;
    }

    for (auto *p : phaseImages)
        delete p;
    phaseImages.clear();

    for (auto *r : radiation)
        delete r;
    radiation.clear();
}

void WorldModel::Reset()
{
    for (auto &mimic : mimicGrid)
        mimic = nullptr;

    for (auto &i : mimicsCaptured)
        i = 0;
}

void WorldModel::Update()
{
    Field::field.contamination += Field::field.contaminationDoT;

    if (Field::field.contaminationDoT > 0.0)
        Field::field.contaminationDoT -= Field::field.contaminationDoTAttenuation;
    if (Field::field.contaminationDoT < 0.0)
        Field::field.contaminationDoT = 0.0;

    if (Field::field.contamination > 0.0)
        Field::field.contamination -= Field::field.contaminationCleanupRate;
    if (Field::field.contamination < 0)
        Field::field.contamination = 0.0;

    Field::field.UpdateContaminationBar();

    if (Field::field.attackCD_current > 0)
        Field::field.attackCD_current--;

    Field::field.ProgressSpawnCD();
    if (Field::field.currentSpawnCD <= 0)
    {
        int mimicsToSpawn = Field::field.SimultaneousSpawnRNG();
        for (int i = 0; i < mimicsToSpawn; i++)
            SpawnMimic();
        Field::field.ResetSpawnCD();
    }

    for (size_t i = 0; i < Field::GRID_CELLS; i++)
    {
        Mimic *occupantMimic = mimicGrid[i];
        if (occupantMimic)
        {
            occupantMimic->Update();

            if (occupantMimic->isCaptured)
            {
                mimicsCaptured[occupantMimic->caste]++;
                delete occupantMimic;
                mimicGrid[i] = nullptr;
            }
            else if (occupantMimic->isExploding)
            {
                delete occupantMimic;
                mimicGrid[i] = nullptr;
                Field::field.contamination += Field::field.contaminationPerLeak;
                Field::field.contaminationDoT += Field::field.contaminationDoTPerLeak;
                SpawnExplosionRadiation(occupantMimic->xPosition, occupantMimic->yPosition);
            }
        }

        if (Field::field.cellUnderAttack[i])
        {
            Field::field.cellAttackProgress[i]++;
            float capturerProgress = static_cast<float>(Field::field.cellAttackProgress[i]) / Field::field.attackNumTicks;
            Field::field.capturerFrame[i] = std::ceil(capturerProgress * (FieldData::CAPTURE_ANIMATION_NUM_FRAMES - 1));

            if (Field::field.cellAttackProgress[i] >= Field::field.attackNumTicks)
            {
                CompleteAttackCell(i);
                Field::field.cellAttackProgress[i] = 0;
                Field::field.cellUnderAttack[i] = false;
            }
        }
    }

    for (const auto &phaseImage : phaseImages)
    {
        phaseImage->location.Update();
        if (phaseImage->location.atDestination)
            phaseImage->active = false;
    }
    auto phase_it = std::remove_if(
        phaseImages.begin(),
        phaseImages.end(),
        [](PhaseImage *phaseImage)
        {
            if (!phaseImage->active)
            {
                delete phaseImage;
                return true;
            }
            return false;
        });
    phaseImages.erase(phase_it, phaseImages.end());

    for (const auto &rad : radiation)
        rad->Update();
    auto rad_it = std::remove_if(
        radiation.begin(),
        radiation.end(),
        [](Radiation *rad)
        {
            if (!rad->isAlive)
            {
                delete rad;
                return true;
            }
            return false;
        });
    radiation.erase(rad_it, radiation.end());
}

void WorldModel::SpawnMimic()
{
    std::vector<int> gridEmptyCells{};

    for (size_t i = 0; i < Field::GRID_CELLS; i++)
        if (!mimicGrid[i])
            gridEmptyCells.push_back(i);

    if (gridEmptyCells.empty())
        return;

    size_t gridEmptyCellsIndex = Random::RandomInt(0, gridEmptyCells.size() - 1);
    size_t gridMimicsIndex = gridEmptyCells[gridEmptyCellsIndex];
    size_t spawnCol = gridMimicsIndex % Field::GRID_COLS;
    size_t spawnRow = gridMimicsIndex / Field::GRID_COLS;

    Mimic *spawnMimic = new Mimic();
    spawnMimic->Initialize(MimicData::CASTE_MOOK);

    spawnMimic->xPosition = Field::field.gridXPosition +
                            spawnCol * FieldData::CELL_WIDTH +
                            FieldData::CELL_WIDTH / 2;

    spawnMimic->yPosition = Field::field.gridYPosition +
                            spawnRow * FieldData::CELL_HEIGHT +
                            FieldData::CELL_HEIGHT / 2;

    mimicGrid[gridMimicsIndex] = spawnMimic;

    PhaseImage *leftPhase = new PhaseImage();
    leftPhase->Initialize(spawnMimic->xPosition - MimicData::PHASING_DISTANCE, spawnMimic->yPosition,
                          spawnMimic->xPosition, spawnMimic->yPosition);

    PhaseImage *rightPhase = new PhaseImage();
    rightPhase->Initialize(spawnMimic->xPosition + MimicData::PHASING_DISTANCE, spawnMimic->yPosition,
                           spawnMimic->xPosition, spawnMimic->yPosition);

    phaseImages.push_back(leftPhase);
    phaseImages.push_back(rightPhase);
}
void WorldModel::SpawnExplosionRadiation(float origin_x, float origin_y)
{
    for (int i = 0; i < 100; i++)
    {
        Radiation *rad = new Radiation();
        rad->Initialize(origin_x, origin_y);
        radiation.push_back(rad);
    }
}
void WorldModel::InitiateAttackCell(size_t cell_index)
{
    if (Field::field.cellUnderAttack[cell_index])
        return;

    Field::field.attackCD_current = Field::field.attackCD_Required;

    Field::field.cellUnderAttack[cell_index] = true;
    Field::field.cellAttackProgress[cell_index] = 0;

    Field::field.capturerFrame[cell_index] = 0;
}

void WorldModel::CompleteAttackCell(size_t cell_index)
{
    Mimic *target = mimicGrid[cell_index];
    if (target && !target->inPhasing)
    {
        target->health--;
        if (target->health <= 0)
            target->isCaptured = true;
    }
    else
        Field::field.contamination += Field::field.contaminationPerMisplay;
}