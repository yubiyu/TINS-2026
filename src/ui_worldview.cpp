#include "ui_worldview.h"

#include "core_timer.h"
#include "core_camera.h"
#include "core_uistate.h"

#include "input_mouse.h"
#include "input_keyboard.h"

#include "resource_image.h"
#include "resource_palette.h"
#include "resource_text.h"

#include "world_worldmodel.h"

#include "data_field.h"

#include "util_drawing.h"

#include <algorithm>

WorldView WorldView::worldView;

void WorldView::Initialize()
{
    UIBase::Initialize(UIData::defaultUIRect[UIData::UI_WORLD_VIEW]);

    //drawDebugPrimitives = true;

    Camera::worldCamera.SetViewDimensions(width, height);
    // Set camera x/y speed here.
}

void WorldView::Uninitialize()
{
    UIBase::Uninitialize();
}

void WorldView::Reset()
{
    Uninitialize();
    Initialize();
}

void WorldView::InputMouse()
{
}
void WorldView::InputKeyboard()
{
    if (Keyboard::Pressed(ALLEGRO_KEY_ESCAPE))
        UIState::exit = true;

    for(int keypadCheck = ALLEGRO_KEY_PAD_1; keypadCheck <= ALLEGRO_KEY_PAD_9; keypadCheck++)
    {
        if(Keyboard::Pressed(keypadCheck))
        {
            int keyIndex = keypadCheck - ALLEGRO_KEY_PAD_1;
            size_t cellIndex = Field::keypadToIndex[keyIndex];

            if(cellIndex < WorldModel::world.mimicGrid.size())
                WorldModel::world.InitiateAttackCell(cellIndex);
        }
    }

    if (Keyboard::Pressed(ALLEGRO_KEY_PAD_5))
    {
    }
}
void WorldView::Update()
{
    // Todo: Add conditions for free camera movement as well as centering the camera on other things.
    // CenterCameraOnPC();

    UpdateBuffer();
}
void WorldView::UpdateBuffer()
{
    ALLEGRO_BITMAP *previousBitmap = al_get_target_bitmap();
    al_set_target_bitmap(buffer);
    al_clear_to_color(Palette::transparent);

    DrawTitle();
    DrawGrid();
    DrawCounters();
    DrawMimics();
    DrawCapturers();

    al_set_target_bitmap(previousBitmap);
}
void WorldView::DrawTitle()
{
    TextUtil::al_draw_string(Text::fieldTitleFont, Palette::colours[Palette::COL_LIGHT],
                             Field::field.titleStringXPosition, Field::field.titleStringYPosition,
                             ALLEGRO_ALIGN_CENTRE, Field::field.titleString);
}
void WorldView::DrawGrid()
{
    al_draw_bitmap(Image::gridFramePng, Field::field.frameXPosition, Field::field.frameYPosition, 0);
    al_draw_bitmap(Image::gridPng, Field::field.gridXPosition, Field::field.gridYPosition, 0);
}
void WorldView::DrawCounters()
{
    size_t mimicIndex = 0;

    // Drawing order: left column first, then right.
    for(size_t drawCol = 0; drawCol < 2; drawCol++)
    {
        for(size_t drawRow = 0; drawRow < 3; drawRow++)
        {
            int spriteDrawX = 64 + 704*drawCol;
            int spriteDrawY = 128 + (128*drawRow);
            int textDrawX = spriteDrawX + 64 + 32;
            int textDrawY = spriteDrawY + Text::FIELD_COUNTER_FONT_HEIGHT/2;
            std::string captureCountString = std::to_string(WorldModel::world.mimicsCaptured[mimicIndex]);

            al_draw_bitmap(Image::mimicAtlas_mimics[mimicIndex], spriteDrawX, spriteDrawY, 0);
            TextUtil::al_draw_string(Text::fieldCounterFont, Palette::textDefault,
                textDrawX, textDrawY,
                ALLEGRO_ALIGN_LEFT, captureCountString);

            mimicIndex++;
        }
    }
}
void WorldView::DrawMimics()
{
    for(size_t i = 0; i < WorldModel::world.mimicGrid.size(); i++)
    {
        Mimic* mimic = WorldModel::world.mimicGrid[i];

        if(!mimic)
            continue;

        size_t mimicCaste = mimic->caste;

        float mimicDrawX = mimic->xPosition - MimicData::SPRITE_WIDTH/2;
        float mimicDrawY = mimic->yPosition - MimicData::SPRITE_HEIGHT/2;
        al_draw_bitmap(Image::mimicAtlas_mimics[mimicCaste], mimicDrawX, mimicDrawY, 0);
    }
}
void WorldView::DrawCapturers()
{
    for(size_t i = 0; i < WorldModel::world.mimicGrid.size(); i++)
    {
        if(!Field::field.cellUnderAttack[i])
            continue;

        int col = i%Field::GRID_COLS;
        int row = i/Field::GRID_COLS;

        float drawX = Field::field.gridXPosition + FieldData::CELL_WIDTH*col;
        float drawY = Field::field.gridYPosition + FieldData::CELL_HEIGHT*row;

        float capturerProgress = static_cast<float>(Field::field.cellAttackProgress[i]) / Field::field.attackNumTicks;
        size_t capturerFrame = capturerProgress * (FieldData::CAPTURE_ANIMATION_NUM_FRAMES - 1);

        al_draw_bitmap(Image::captureAtlas[capturerFrame], drawX, drawY, 0);
        
    }
}
