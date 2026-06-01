#include "world_worldmodel.h"

#include "core_uistate.h"
#include "core_random.h"
#include "core_display.h"

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

    for (auto *m : looseMimics)
        delete m;
    looseMimics.clear();
}

void WorldModel::Reset()
{
    gameFailed = false;
    gameProgress = 0;
    usingMaxSpawnableClade = true;

    /*
    Debug begin.
    */
    // gameFailed = true; // Comment out.
    // usingMaxSpawnableClade = false; // Comment out.
    /*
    Debug end.
    */

    WorldModel::Uninitialize();
    Field::field.Initialize();

    for (auto &i : mimicsCaptured)
        i = 0;
    for (auto &i : chaosScore)
        i = 0;

    SetDefaultRedirectionArray();
}

void WorldModel::Update()
{
    Field::field.Update();
    Field::field.ProgressSpawnCD();

    if (gameFailed)
        Field::field.currentSpawnCD -= 2; // More chaos!

    if (Field::field.currentSpawnCD <= 0)
    {
        int mimicsToSpawn = Field::field.SimultaneousSpawnRNG();
        for (int i = 0; i < mimicsToSpawn; i++)
        {
            if (!gameFailed)
                SpawnMimicToGrid();
            else
                SpawnMimicBreached();
        }
        Field::field.ResetSpawnCD();
    }

    for (size_t i = 0; i < Field::GRID_CELLS; i++)
    {
        Mimic *occupantMimic = mimicGrid[i];
        if (occupantMimic)
        {
            occupantMimic->Update();

            if (occupantMimic->isCaptured || occupantMimic->isDefused)
            {
                AddCapture(occupantMimic->clade);
                delete occupantMimic;
                mimicGrid[i] = nullptr;

                UpdateRedirectionArray();
            }
            else if (occupantMimic->isExploding)
            {
                delete occupantMimic;
                mimicGrid[i] = nullptr;
                Leak();
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

    for (const auto &breachMimic : looseMimics)
    {
        breachMimic->Update();
        if (breachMimic->isDefused || breachMimic->isExploding)
        {
            Leak();
            if (breachMimic->isStunner)
                SpawnStunLightning(breachMimic->xPosition, breachMimic->yPosition);
            else
                SpawnExplosionRadiation(breachMimic->xPosition, breachMimic->yPosition);
        }
    }
    auto loose_it = std::remove_if(
        looseMimics.begin(),
        looseMimics.end(),
        [](Mimic *looseMimic)
        {
            if (looseMimic->isDefused || looseMimic->isExploding)
            {
                delete looseMimic;
                return true;
            }
            return false;
        });
    looseMimics.erase(loose_it, looseMimics.end());

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

    for (const auto &spark : stunLightnings)
        spark->Update();
    auto spark_it = std::remove_if(
        stunLightnings.begin(),
        stunLightnings.end(),
        [](StunLightning *spark)
        {
            if (!spark->isAlive)
            {
                delete spark;
                return true;
            }
            return false;
        });
    stunLightnings.erase(spark_it, stunLightnings.end());

    if (gameFailed)
    {
        chaosScoreTicks_current++;
        if (chaosScoreTicks_current >= chaosScoreTicks_Max)
        {
            chaosScoreTicks_current = 0;
            for (auto &i : chaosScore)
            {
                if (Random::FlipCoin())
                    i = Random::RandomInt(1000, 9999);
            }
        }
    }
    else
    {
        if (Field::field.contamination >= 100)
            Field::field.inCriticalState = true;

        if (Field::field.inCriticalState)
        {
            Field::field.overflowGrace_current--;
            if (Field::field.overflowGrace_current <= 0)
                gameFailed = true;

            if(Field::field.contamination < 100)
                Field::field.ResetCriticalState();
        }


    }
}

void WorldModel::SpawnMimicToGrid()
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

    int maxSpawnableClade = MimicData::CLADE_VARIABLE;
    if (usingMaxSpawnableClade)
        while (gameProgress < MimicData::progressToEncounterClade[maxSpawnableClade])
            maxSpawnableClade--; // Repeatedly adjust downward to a suitable level.
    size_t cladeRoll = Random::RandomInt(MimicData::CLADE_MOOK, maxSpawnableClade);

    /*debug*/
    // cladeRoll = MimicData::CLADE_STUNNER;
    /*end debug*/

    spawnMimic->Initialize(cladeRoll);
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
            spawnMimic->Initialize(MimicData::CLADE_MOOK); // No room to spawn the new guy.
    }

    spawnMimic->xPosition = Field::field.gridXPosition +
                            spawnCol * FieldData::CELL_WIDTH +
                            FieldData::CELL_WIDTH / 2;

    spawnMimic->yPosition = Field::field.gridYPosition +
                            spawnRow * FieldData::CELL_HEIGHT +
                            FieldData::CELL_HEIGHT / 2;

    PhaseImage *leftPhase = new PhaseImage();
    leftPhase->Initialize(spawnMimic->clade,
                          spawnMimic->xPosition - MimicData::PHASING_DISTANCE, spawnMimic->yPosition,
                          spawnMimic->xPosition, spawnMimic->yPosition);

    PhaseImage *rightPhase = new PhaseImage();
    rightPhase->Initialize(spawnMimic->clade,
                           spawnMimic->xPosition + MimicData::PHASING_DISTANCE, spawnMimic->yPosition,
                           spawnMimic->xPosition, spawnMimic->yPosition);

    phaseImages.push_back(leftPhase);
    phaseImages.push_back(rightPhase);
}
void WorldModel::SpawnMimicBreached()
{
    Mimic *breachMimic = new Mimic();
    int maxSpawnableClade = MimicData::CLADE_VARIABLE;
    if (usingMaxSpawnableClade)
        while (gameProgress < MimicData::progressToEncounterClade[maxSpawnableClade])
            maxSpawnableClade--; // Repeatedly adjust downward to a suitable level.
    size_t cladeRoll = Random::RandomInt(MimicData::CLADE_MOOK, maxSpawnableClade);
    breachMimic->Initialize(cladeRoll);

    if (breachMimic->isRedirector)
        breachMimic->SetRedirectionIndex(Random::RandomInt(0, 8));

    breachMimic->xPosition = Random::RandomInt(64, Display::width - 64);
    breachMimic->yPosition = Random::RandomInt(64, Display::height - 64);

    looseMimics.push_back(breachMimic);

    PhaseImage *leftPhase = new PhaseImage();
    leftPhase->Initialize(breachMimic->clade,
                          breachMimic->xPosition - MimicData::PHASING_DISTANCE, breachMimic->yPosition,
                          breachMimic->xPosition, breachMimic->yPosition);

    PhaseImage *rightPhase = new PhaseImage();
    rightPhase->Initialize(breachMimic->clade,
                           breachMimic->xPosition + MimicData::PHASING_DISTANCE, breachMimic->yPosition,
                           breachMimic->xPosition, breachMimic->yPosition);

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
        if (mimicGrid[adjacentCells[i]]) // Already occupied, bud.
            continue;

        if (Random::RandomInt(1, 4) != 4)
            continue;

        Mimic *splitter = new Mimic();
        splitter->Initialize(MimicData::CLADE_SPLITTER);
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
        leftPhase->Initialize(splitter->clade,
                              splitter->xPosition - MimicData::PHASING_DISTANCE, splitter->yPosition,
                              splitter->xPosition, splitter->yPosition);
        PhaseImage *rightPhase = new PhaseImage();
        rightPhase->Initialize(splitter->clade,
                               splitter->xPosition + MimicData::PHASING_DISTANCE, splitter->yPosition,
                               splitter->xPosition, splitter->yPosition);
        phaseImages.push_back(leftPhase);
        phaseImages.push_back(rightPhase);
    }
}
void WorldModel::SpawnExplosionRadiation(float origin_x, float origin_y)
{
    for (int i = 0; i < 50; i++)
    {
        Radiation *rad = new Radiation();
        rad->Initialize(origin_x, origin_y, false);
        radiations.push_back(rad);
    }

    for (int i = 0; i < 10; i++)
    {
        Radiation *sharps = new Radiation();
        sharps->Initialize(origin_x, origin_y, true);
        radiations.push_back(sharps);
    }
}
void WorldModel::SpawnStunLightning(float origin_x, float origin_y)
{
    int numSparks = Random::RandomInt(1, 5);
    for (int i = 0; i < numSparks; i++)
    {
        StunLightning *spark = new StunLightning();
        spark->Initialize(origin_x, origin_y);
        stunLightnings.push_back(spark);
    }
}
void WorldModel::InitiateAttackCell(size_t cell_index)
{
    if (Field::field.isStunned)
        return;

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
                leftPhase->Initialize(target->clade,
                                      target->xPosition, target->yPosition,
                                      target->xPosition - MimicData::PHASING_DISTANCE, target->yPosition);
                PhaseImage *rightPhase = new PhaseImage();
                rightPhase->Initialize(target->clade,
                                       target->xPosition, target->yPosition,
                                       target->xPosition + MimicData::PHASING_DISTANCE, target->yPosition);
                phaseImages.push_back(leftPhase);
                phaseImages.push_back(rightPhase);

                SpawnMimic_Splitters(cell_index % Field::GRID_COLS,
                                     cell_index / Field::GRID_ROWS);
            }

            if (target->isStunner)
            {
                Field::field.Stun();
                SpawnStunLightning(target->xPosition, target->yPosition);
                Misplay();
            }
            target->isCaptured = true;
        }
    }
    else
        Misplay();
}
void WorldModel::AddCapture(size_t which_clade)
{
    mimicsCaptured[which_clade]++;
    gameProgress++;
}

void WorldModel::Misplay()
{
    Field::field.contamination += Field::field.contaminationPerMisplay;
    Field::field.BeginContaminationFlicker();
}
void WorldModel::Leak()
{
    Field::field.contamination += Field::field.contaminationPerLeak;
    Field::field.contaminationDoT += Field::field.contaminationDoTPerLeak;
    Field::field.BeginContaminationFlicker();
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