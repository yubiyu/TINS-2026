#include "world_worldmodel.h"

#include "core_uistate.h"
#include "core_random.h"

#include "ui_worldview.h"

#include "data_field.h"

#include <iostream>

WorldModel WorldModel::world;

void WorldModel::Initialize()
{
    Field::field.Initialize();
}

void WorldModel::Uninitialize()
{
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
    Field::field.ProgressSpawnCD();
    if (Field::field.currentSpawnCD <= 0)
    {
        int mimicsToSpawn = Field::field.SimultaneousSpawnRNG();
        for(int i = 0; i < mimicsToSpawn; i++)
            SpawnMimic();
        Field::field.ResetSpawnCD();
    }

    for(size_t i = 0; i < Field::GRID_CELLS; i++)
    {
        if(Field::field.cellUnderAttack[i])
        {
            Field::field.cellAttackProgress[i] ++;
            if(Field::field.cellAttackProgress[i] >= Field::field.attackNumTicks)
            {
                AttackCell(i);
                Field::field.cellAttackProgress[i] = 0;
                Field::field.cellUnderAttack[i] = false;
            }
        }
    }
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
    spawnMimic->Initialize(MimicData::CASTE_DRONE);

    spawnMimic->xPosition = Field::field.gridXPosition +
                            spawnCol * FieldData::CELL_WIDTH +
                            FieldData::CELL_WIDTH/2;

    spawnMimic->yPosition = Field::field.gridYPosition +
                            spawnRow * FieldData::CELL_HEIGHT +
                            FieldData::CELL_HEIGHT/2;

    mimicGrid[gridMimicsIndex] = spawnMimic;
}

void WorldModel::InitiateAttackCell(size_t cell_index)
{
    Field::field.cellUnderAttack[cell_index] = true;
    Field::field.cellAttackProgress[cell_index] = 0;
}

void WorldModel::AttackCell(size_t cell_index)
{
    std::cout << "Attack cell " << cell_index << std::endl;

    Mimic* target = mimicGrid[cell_index];
    if(target)
    {
        delete target;
        mimicGrid[cell_index] = nullptr;
    }
}