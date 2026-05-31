#include "world_field.h"

#include "data_field.h"

#include "core_display.h"
#include "core_timer.h"
#include "core_random.h"

#include "resource_text.h"

#include <vector>

Field Field::field;

void Field::Initialize()
{
    gridWidth = FieldData::CELL_WIDTH * 3;
    gridHeight = FieldData::CELL_HEIGHT * 3;
    gridXPosition = Display::width / 2 - gridWidth / 2;
    gridYPosition = Display::height / 2 - gridHeight / 2;

    gridFrameWidth = gridWidth + 4;
    gridFrameHeight = gridHeight + 8;
    gridFrameXPosition = gridXPosition - 4;
    gridFrameYPosition = gridYPosition - 8;

    for (size_t i = 0; i < GRID_CELLS; i++)
    {
        int col = i % Field::GRID_COLS;
        int row = i / Field::GRID_COLS;
        float x = gridXPosition + FieldData::CELL_WIDTH * col;
        float y = gridYPosition + FieldData::CELL_HEIGHT * row;

        cellXYPosition[i] = {x, y};
    }

    revertButtonWidth = 64;
    revertButtonXPosition = gridXPosition;
    revertButtonYPosition = gridYPosition + gridHeight;
    revertButtonFrameXPosition = revertButtonXPosition - 4;
    revertButtonFrameYPosition = revertButtonYPosition;

    tachyonBarXPosition = revertButtonXPosition + revertButtonWidth;
    tachyonBarYPosition = revertButtonYPosition;

    tachyonBarMaxWidth = 320;
    tachyonBarHeight = 64;
    titleStringXPosition = Display::width / 2;
    titleStringYPosition = gridFrameYPosition / 2 - Text::FIELD_TITLE_FONT_HEIGHT / 2;

    tachyonBarPhaseShift_Max = 32 - 1;
    tachyonBarPhaseShift_Change = (tachyonBarPhaseShift_Max / Timer::FPS) * 1.5;

    // Centered coords.
    int dialogCX = Display::width/2;
    int dialogCY = Display::height/2;
    dialogFrameXY = {dialogCX, dialogCY};
    dialogErrorXY = {dialogCX, dialogCY - 64};
    dialogGravimetricInterferenceXY = {dialogCX, dialogCY};
    dialogRecalibratingXY = {dialogCX, dialogCY + 64};



    dialogDisplacementTicks_max = Timer::FPS * 0.05;

    Reset();
}

void Field::Reset()
{
    titleString = "mimic_suppression_field";

    contamination = 0.0;
    contaminationDoT = 0.2;

    contaminationPerLeak = 10.0;
    contaminationDoTPerLeak = 0.1;

    contaminationPerMisplay = 10.0;

    contaminationCleanupRate = 1.2 / Timer::FPS;
    contaminationDoTAttenuation = contaminationDoTPerLeak / (Timer::FPS * 2);

    isStunned = false;
    stunRecovery_Max = Timer::FPS * 1.5;
    stunRecovery_current = 0;

    UpdateContaminationBar();

    attackNumTicks = Timer::FPS * 0.25;
    attackCD_Required = Timer::FPS * 0.25;
    attackCD_current = 0;

    for (size_t i = 0; i < GRID_CELLS; i++)
    {
        cellUnderAttack[i] = false;
        cellAttackProgress[i] = 0;

        capturerFrame[i] = 0;
    }

    baselineSpawnCD = Timer::FPS * 1.5;
    spawnCDLowerLimit = baselineSpawnCD - Timer::FPS * 0.5;
    spawnCDUpperLimit = baselineSpawnCD + Timer::FPS * 0.5;

    currentSpawnCD = spawnCDLowerLimit;
}
void Field::Update()
{
    if (isStunned)
    {
        stunRecovery_current++;
        if (stunRecovery_current >= stunRecovery_Max)
            isStunned = false;

        dialogDisplacementTicks_current++;
        if (dialogDisplacementTicks_current >= dialogDisplacementTicks_max)
        {
            dialogDisplacementTicks_current = 0;
            DisplaceStunDialog();
        }

    }

    contamination += contaminationDoT;

    if (contaminationDoT > 0.0)
        contaminationDoT -= contaminationDoTAttenuation;
    if (contaminationDoT < 0.0)
        contaminationDoT = 0.0;

    if (contamination > 0.0)
        contamination -= contaminationCleanupRate;
    if (contamination < 0)
        contamination = 0.0;

    UpdateContaminationBar();

    if (attackCD_current > 0)
        attackCD_current--;
}
void Field::ResetSpawnCD()
{
    currentSpawnCD = Random::RandomInt(spawnCDLowerLimit, spawnCDUpperLimit);
}
void Field::ProgressSpawnCD()
{
    currentSpawnCD--;
}
int Field::SimultaneousSpawnRNG()
{
    int mimicsToSpawn = 0;
    int roll = Random::RandomInt(1, 100);

    if (roll <= 50)
        mimicsToSpawn = 1;
    else if (roll <= 85)
        mimicsToSpawn = 2;
    else if (roll <= 100)
        mimicsToSpawn = 3;

    return mimicsToSpawn;
}

void Field::UpdateContaminationBar()
{
    tachyonBarCurrentWidth = tachyonBarMaxWidth * (contamination / 100.0);
    if (tachyonBarCurrentWidth > tachyonBarMaxWidth)
        tachyonBarCurrentWidth = tachyonBarMaxWidth;

    tachyonBarPhaseShift_Current += tachyonBarPhaseShift_Change;
    if (tachyonBarPhaseShift_Current >= tachyonBarMaxWidth)
        tachyonBarPhaseShift_Current -= tachyonBarMaxWidth;
}

void Field::Stun()
{
    isStunned = true;
    stunRecovery_current = 0;

    dialogDisplacementTicks_current = 0;
}

void Field::DisplaceStunDialog()
{
    const int displaceMin = -8;
    const int displaceMax = +8;

    dialogFrameDisplacement =
    {
        Random::RandomInt(displaceMin, displaceMax),
        Random::RandomInt(displaceMin, displaceMax)
    };

    dialogErrorDisplacement = 
    {
        Random::RandomInt(displaceMin, displaceMax),
        Random::RandomInt(displaceMin, displaceMax)
    };

    dialogGravimetricInterferenceDisplacement = 
    {
        Random::RandomInt(displaceMin, displaceMax),
        Random::RandomInt(displaceMin, displaceMax)
    };

    dialogRecalibratingDisplacement = 
    {
        Random::RandomInt(displaceMin, displaceMax),
        Random::RandomInt(displaceMin, displaceMax)
    };
}