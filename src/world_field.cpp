#include "world_field.h"

#include "core_display.h"
#include "core_timer.h"
#include "core_random.h"

#include "resource_text.h"

#include <vector>

Field Field::field;

void Field::Initialize()
{
    gridWidth = 96*4;
    gridHeight = 96*4;
    gridXPosition = Display::width/2 - gridWidth/2;
    gridYPosition = Display::height/2 - gridHeight/2;

    frameWidth = gridWidth + 4;
    frameHeight = gridHeight + 4;
    frameXPosition = gridXPosition - 4;
    frameYPosition = gridYPosition - 4;

    titleStringXPosition = Display::width/2;
    titleStringYPosition = frameYPosition/2 - Text::FIELD_TITLE_FONT_HEIGHT/2;

    Reset();
}

void Field::Reset()
{
    titleString = "mimic_suppression_field";

    attackNumTicks = Timer::FPS * 0.25;

    for(size_t i = 0; i < GRID_CELLS; i++)
    {
        cellUnderAttack[i] = false;
        cellAttackProgress[i] = 0;
    }

    baselineSpawnCD = Timer::FPS * 0.5;
    spawnCDLowerLimit = baselineSpawnCD;
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