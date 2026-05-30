#include "world_field.h"

#include "core_display.h"

#include "resource_text.h"

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

    titleString = "mimic_suppression_field";
    titleStringXPosition = Display::width/2;
    titleStringYPosition = frameYPosition/2 - Text::FIELD_TITLE_FONT_HEIGHT/2;

    Reset();
    
}

void Field::Reset()
{
    for(auto& mimic : gridMimics)
        mimic = nullptr;

    for(auto& i : mimicsCaptured)
        i = 0;
}