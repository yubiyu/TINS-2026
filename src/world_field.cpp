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
    gridWidth = FieldData::CELL_WIDTH*3;
    gridHeight = FieldData::CELL_HEIGHT*3;
    gridXPosition = Display::width/2 - gridWidth/2;
    gridYPosition = Display::height/2 - gridHeight/2;

    gridFrameWidth = gridWidth + 4;
    gridFrameHeight = gridHeight + 4;
    gridFrameXPosition = gridXPosition - 4;
    gridFrameYPosition = gridYPosition - 4;


    polarityButtonWidth = 64;
    polarityButtonXPosition = gridXPosition;
    polarityButtonYPosition = gridYPosition + gridHeight;
    polarityButtonFrameXPosition = polarityButtonXPosition - 4;
    polarityButtonFrameYPosition = polarityButtonYPosition;

    tachyonBarXPosition = polarityButtonXPosition + polarityButtonWidth;
    tachyonBarYPosition = polarityButtonYPosition;

    titleStringXPosition = Display::width/2;
    titleStringYPosition = gridFrameYPosition/2 - Text::FIELD_TITLE_FONT_HEIGHT/2;


    for(size_t i = 0; i < GRID_CELLS; i++)
    {
        int col = i%Field::GRID_COLS;
        int row = i/Field::GRID_COLS;
        float x = Field::field.gridXPosition + FieldData::CELL_WIDTH*col;
        float y = Field::field.gridYPosition + FieldData::CELL_HEIGHT*row;

        cellXYPosition[i] = {x,y};
    }

    Reset();
}

void Field::Reset()
{
    titleString = "mimic_suppression_field";

    attackNumTicks = Timer::FPS * 0.25;
    attackCD_Required = Timer::FPS * 0.25;
    attackCD_current = 0;

    for(size_t i = 0; i < GRID_CELLS; i++)
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
    int roll = Random::RandomInt(1,100);

    if(roll <= 50)
        mimicsToSpawn = 1;
    else if(roll <= 85)
        mimicsToSpawn = 2;
    else if(roll <= 100)
        mimicsToSpawn = 3;
    
    return mimicsToSpawn;
}