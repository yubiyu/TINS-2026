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
    Reset();
}

void WorldModel::Uninitialize()
{
    for (size_t i = 0; i < mimicGrid.size(); i++)
    {
        if (mimicGrid[i])
            delete mimicGrid[i];

        mimicGrid[i] = nullptr;
    }

    for (auto *p : phaseImages)
        delete p;
    phaseImages.clear();

    for (auto *r : radiations)
        delete r;
    radiations.clear();

    for (auto *l : stunLightnings)
        delete l;
    stunLightnings.clear();
}

void WorldModel::Reset()
{
    WorldModel::Uninitialize();

    Field::field.Initialize();

    for (auto &mimic : mimicGrid)
        mimic = nullptr;

    for (auto &i : mimicsCaptured)
        i = 0;

    SetDefaultRedirectionArray();
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

                UpdateRedirectionArray();
            }
            else if (occupantMimic->isExploding)
            {
                delete occupantMimic;
                mimicGrid[i] = nullptr;
                Field::field.contamination += Field::field.contaminationPerLeak;
                Field::field.contaminationDoT += Field::field.contaminationDoTPerLeak;
                SpawnExplosionRadiation(occupantMimic->xPosition, occupantMimic->yPosition);

                UpdateRedirectionArray();
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

    for (const auto &rad : radiations)
        rad->Update();
    auto rad_it = std::remove_if(
        radiations.begin(),
        radiations.end(),
        [](Radiation *rad)
        {
            if (!rad->isAlive)
            {
                delete rad;
                return true;
            }
            return false;
        });
    radiations.erase(rad_it, radiations.end());

    for ( const auto &spark : stunLightnings)
        spark->Update();
    auto spark_it = std::remove_if(
        stunLightnings.begin(),
        stunLightnings.end(),
        [](StunLightning *spark)
        {
            if(!spark->isAlive)
            {
                delete spark;
                return true;
            }
            return false;
        });
    stunLightnings.erase(spark_it, stunLightnings.end());
}

void WorldModel::DeleteInactives()
{
    
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
    mimicGrid[gridMimicsIndex] = spawnMimic;

    size_t spawnRoll = Random::RandomInt(MimicData::CASTE_MOOK, MimicData::CASTE_STUNNER);

    spawnMimic->Initialize(spawnRoll);
    if (spawnMimic->isRedirector)
    {
        std::vector<size_t> redirectableCells;
        redirectableCells.reserve(Field::GRID_CELLS);
        for (size_t i = 0; i < redirectionArray.size(); i++)
            if (redirectionArray[i] == i && i != gridMimicsIndex) // i.e. hasn't been changed from its default value, and not about to redirect to itself.
                redirectableCells.push_back(i);

        if (!redirectableCells.empty())
        {
            size_t redirectionRoll = Random::RandomInt(0, redirectableCells.size() - 1);
            spawnMimic->SetRedirectionIndex(redirectableCells[redirectionRoll]);
            UpdateRedirectionArray();
        }
        else
        {
            spawnMimic->Initialize(MimicData::CASTE_MOOK);
        }
    }

    spawnMimic->xPosition = Field::field.gridXPosition +
                            spawnCol * FieldData::CELL_WIDTH +
                            FieldData::CELL_WIDTH / 2;

    spawnMimic->yPosition = Field::field.gridYPosition +
                            spawnRow * FieldData::CELL_HEIGHT +
                            FieldData::CELL_HEIGHT / 2;

    PhaseImage *leftPhase = new PhaseImage();
    leftPhase->Initialize(spawnMimic->xPosition - MimicData::PHASING_DISTANCE, spawnMimic->yPosition,
                          spawnMimic->xPosition, spawnMimic->yPosition);

    PhaseImage *rightPhase = new PhaseImage();
    rightPhase->Initialize(spawnMimic->xPosition + MimicData::PHASING_DISTANCE, spawnMimic->yPosition,
                           spawnMimic->xPosition, spawnMimic->yPosition);

    phaseImages.push_back(leftPhase);
    phaseImages.push_back(rightPhase);
}
void WorldModel::SpawnMimic_Splitters(int origin_col, int origin_row)
{
    /*
    The hackiest garbage function of all time.
    */

    std::vector<size_t> adjacentCells{};
    if (origin_row > 0)
        adjacentCells.push_back((origin_row - 1) * Field::GRID_COLS + origin_col);
    if (origin_row < 2)
        adjacentCells.push_back((origin_row + 1) * Field::GRID_COLS + origin_col);

    if (origin_col > 0)
        adjacentCells.push_back(origin_row * Field::GRID_COLS + (origin_col - 1));
    if (origin_col < 2)
        adjacentCells.push_back(origin_row * Field::GRID_COLS + (origin_col + 1));

    for (size_t i = 0; i < adjacentCells.size(); i++)
    {
        if(mimicGrid[adjacentCells[i]]) // Already occupied, bud.
            continue;

        if (Random::RandomInt(1, 4) != 4)
            continue;

        Mimic *splitter = new Mimic();
        splitter->Initialize(MimicData::CASTE_SPLITTER);
        mimicGrid[adjacentCells[i]] = splitter;


        int spawnCol = adjacentCells[i] % Field::GRID_COLS;
        int spawnRow = adjacentCells[i] / Field::GRID_COLS;

        splitter->xPosition = Field::field.gridXPosition +
                              spawnCol * FieldData::CELL_WIDTH +
                              FieldData::CELL_WIDTH / 2;

        splitter->yPosition = Field::field.gridYPosition +
                              spawnRow * FieldData::CELL_HEIGHT +
                              FieldData::CELL_HEIGHT / 2;

        PhaseImage *leftPhase = new PhaseImage();
        leftPhase->Initialize(splitter->xPosition - MimicData::PHASING_DISTANCE, splitter->yPosition,
                              splitter->xPosition, splitter->yPosition);
        PhaseImage *rightPhase = new PhaseImage();
        rightPhase->Initialize(splitter->xPosition + MimicData::PHASING_DISTANCE, splitter->yPosition,
                               splitter->xPosition, splitter->yPosition);
        phaseImages.push_back(leftPhase);
        phaseImages.push_back(rightPhase);
    }
}
void WorldModel::SpawnExplosionRadiation(float origin_x, float origin_y)
{
    for (int i = 0; i < 100; i++)
    {
        Radiation *rad = new Radiation();
        rad->Initialize(origin_x, origin_y);
        radiations.push_back(rad);
    }
}
void WorldModel::SpawnStunLightning(float origin_x, float origin_y)
{
    std::cout << "Stun lightning " << origin_x << "," << origin_y << std::endl;
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
        {
            if (target->isSplitter)
            {
                PhaseImage *leftPhase = new PhaseImage();
                leftPhase->Initialize(target->xPosition, target->yPosition,
                                      target->xPosition - MimicData::PHASING_DISTANCE, target->yPosition);
                PhaseImage *rightPhase = new PhaseImage();
                rightPhase->Initialize(target->xPosition, target->yPosition,
                                       target->xPosition + MimicData::PHASING_DISTANCE, target->yPosition);
                phaseImages.push_back(leftPhase);
                phaseImages.push_back(rightPhase);

                SpawnMimic_Splitters(cell_index % Field::GRID_COLS,
                                     cell_index / Field::GRID_ROWS);
            }
            
            if( target->isStunner)
            {
                Field::field.Stun();
                SpawnStunLightning(target->xPosition, target->yPosition);
            }
            target->isCaptured = true;
        }
    }
    else
        Field::field.contamination += Field::field.contaminationPerMisplay;
}

void WorldModel::UpdateRedirectionArray()
{
    SetDefaultRedirectionArray();
    std::array<int, Field::GRID_CELLS> redirectionChanges;
    redirectionChanges.fill(-1);

    for (size_t i = 0; i < Field::GRID_CELLS; i++)
    {
        Mimic *checkRedirector = mimicGrid[i];
        if (!checkRedirector)
            continue;

        if (checkRedirector->isRedirector)
        {
            redirectionChanges[i] = checkRedirector->redirectionIndex;
            redirectionChanges[checkRedirector->redirectionIndex] = i;
        }
    }

    for (size_t i = 0; i < Field::GRID_CELLS; i++)
    {
        if (redirectionChanges[i] == -1)
            continue;

        redirectionArray[i] = redirectionChanges[i];
    }
}
void WorldModel::SetDefaultRedirectionArray()
{
    redirectionArray = {0, 1, 2, 3, 4, 5, 6, 7, 8};
}