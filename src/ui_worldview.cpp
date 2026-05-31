#include "ui_worldview.h"

#include "core_display.h"
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

#include <allegro5/allegro5.h>

#include <algorithm>
#include <cmath>

WorldView WorldView::worldView;

void WorldView::Initialize()
{
    UIBase::Initialize(UIData::defaultUIRect[UIData::UI_WORLD_VIEW]);

    // drawDebugPrimitives = true;

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

    for (int keypadCheck = ALLEGRO_KEY_PAD_1; keypadCheck <= ALLEGRO_KEY_PAD_9; keypadCheck++)
    {
        if (Field::field.attackCD_current == 0 && Keyboard::Pressed(keypadCheck))
        {
            int keyIndex = keypadCheck - ALLEGRO_KEY_PAD_1;
            size_t cellIndex = Field::keypadToIndex[keyIndex];

            size_t redirectedCellIndex = WorldModel::world.redirectionArray[cellIndex];

            WorldModel::world.InitiateAttackCell(redirectedCellIndex);
        }
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
    DrawPhaseImages();
    DrawMimics();
    DrawCapturers();
    DrawStunLightnings();
    DrawRadiation();
    DrawDialog();

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
    al_draw_bitmap(Image::gridFramePng, Field::field.gridFrameXPosition, Field::field.gridFrameYPosition, 0);
    al_draw_bitmap(Image::gridPng, Field::field.gridXPosition, Field::field.gridYPosition, 0);

    al_draw_bitmap(Image::revertButtonPng, Field::field.revertButtonXPosition, Field::field.revertButtonYPosition, 0);
    al_draw_bitmap(Image::revertButtonFramePng, Field::field.revertButtonFrameXPosition, Field::field.revertButtonFrameYPosition, 0);

    // Barberpole effect using two rectangular draw regions.
    float firstPart = std::min(Field::field.tachyonBarCurrentWidth, Field::field.tachyonBarMaxWidth - Field::field.tachyonBarPhaseShift_Current);
    al_draw_bitmap_region(
        Image::tachyonBarPng,
        Field::field.tachyonBarPhaseShift_Current, 0,
        firstPart,
        Field::field.tachyonBarHeight,
        Field::field.tachyonBarXPosition,
        Field::field.tachyonBarYPosition,
        0);
    float secondPart = Field::field.tachyonBarCurrentWidth - firstPart;
    if (secondPart > 0)
    {
        al_draw_bitmap_region(
            Image::tachyonBarPng,
            0, 0,
            secondPart,
            Field::field.tachyonBarHeight,
            Field::field.tachyonBarXPosition + firstPart,
            Field::field.tachyonBarYPosition,
            0);
    }

    al_draw_bitmap(Image::tachyonBarFramePng, Field::field.tachyonBarXPosition, Field::field.tachyonBarYPosition, 0);

    if (Field::field.tachyonBarCurrentWidth > 0.0 && Field::field.tachyonBarCurrentWidth < Field::field.tachyonBarMaxWidth)
    {
        float x1 = Field::field.tachyonBarXPosition + Field::field.tachyonBarCurrentWidth;
        float y1 = Field::field.tachyonBarYPosition;
        float x2 = x1 + 4;
        float y2 = y1 + Field::field.tachyonBarHeight - 8;
        al_draw_filled_rectangle(x1, y1, x2, y2, Palette::colours[Palette::COL_BLACK]);
    }
}
void WorldView::DrawCounters()
{
    size_t mimicIndex = 0;

    // Drawing order: left column first, then right.
    for (size_t drawCol = 0; drawCol < 2; drawCol++)
    {
        for (size_t drawRow = 0; drawRow < 3; drawRow++)
        {
            int spriteDrawX = 64 + 704 * drawCol;
            int spriteDrawY = 128 + (128 * drawRow);
            int textDrawX = spriteDrawX + 64 + 32;
            int textDrawY = spriteDrawY + Text::FIELD_COUNTER_FONT_HEIGHT / 2;
            std::string captureCountString = std::to_string(WorldModel::world.mimicsCaptured[mimicIndex]);

            if (WorldModel::world.mimicsCaptured[mimicIndex] >= 1)
            {
                al_draw_bitmap(Image::mimicAtlas_mimicsB[mimicIndex], spriteDrawX, spriteDrawY, 0);
                al_draw_bitmap(Image::pupilAtlas[MimicData::PUPIL_DOT * MimicData::NUM_PUPIL_VARIANTS + 0], spriteDrawX, spriteDrawY, 0);
            }
            else
                al_draw_bitmap(Image::mimicAtlas_unknownMimics[mimicIndex], spriteDrawX, spriteDrawY, 0);

            TextUtil::al_draw_string(Text::fieldCounterFont, Palette::textDefault,
                                     textDrawX, textDrawY,
                                     ALLEGRO_ALIGN_LEFT, captureCountString);

            mimicIndex++;
        }
    }
}
void WorldView::DrawPhaseImages()
{
    for (const auto &phaseImage : WorldModel::world.phaseImages)
    {
        size_t drawCaste = phaseImage->caste;
        float drawX = phaseImage->location.current.x - MimicData::SPRITE_WIDTH / 2;
        float drawY = phaseImage->location.current.y - MimicData::SPRITE_HEIGHT / 2;
        al_draw_bitmap(Image::mimicAtlas_phasingMimics[drawCaste], drawX, drawY, 0);
    }
}
void WorldView::DrawMimics()
{
    for (size_t i = 0; i < WorldModel::world.mimicGrid.size(); i++)
    {
        Mimic *mimic = WorldModel::world.mimicGrid[i];

        if (!mimic)
            continue;

        if (mimic->inPhasing)
            continue;

        size_t mimicCaste = mimic->caste;

        /*
        if (Field::field.cellUnderAttack[i])
        {
            int capturerX = Field::field.cellXYPosition[i].x;
            int capturerY = Field::field.cellXYPosition[i].y;
            size_t capturerFrame = Field::field.capturerFrame[i];
            ALLEGRO_BITMAP *previousBitmap = al_get_target_bitmap();
            ALLEGRO_BITMAP *maskBuffer = al_create_bitmap(FieldData::CELL_WIDTH, FieldData::CELL_HEIGHT);
            al_set_target_bitmap(maskBuffer);
            al_draw_bitmap(Image::captureAtlas_mask[capturerFrame], 0, 0, 0);
            al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ZERO);
            al_draw_bitmap(Image::mimicAtlas_mimics[mimicCaste],
                           FieldData::CELL_WIDTH / 2 - MimicData::SPRITE_WIDTH / 2,
                           FieldData::CELL_HEIGHT / 2 - MimicData::SPRITE_HEIGHT / 2,
                           0);
            al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
            al_set_target_bitmap(previousBitmap);
            al_draw_bitmap(maskBuffer, capturerX, capturerY, 0);
            al_destroy_bitmap(maskBuffer);
        }
        else
        {
        */
        float mimicDrawX = mimic->xPosition - MimicData::SPRITE_WIDTH / 2;
        float mimicDrawY = mimic->yPosition - MimicData::SPRITE_HEIGHT / 2;
        if (mimic->inFrameB)
            al_draw_bitmap(Image::mimicAtlas_mimicsB[mimicCaste], mimicDrawX, mimicDrawY, 0);
        else
            al_draw_bitmap(Image::mimicAtlas_mimicsA[mimicCaste], mimicDrawX, mimicDrawY, 0);

        size_t pupilIndex = mimic->pupilSpriteIndex;
        float pupilDrawX = mimicDrawX + mimic->pupilXDisplacement;
        float pupilDrawY = mimicDrawY + mimic->pupilYDisplacement;

        al_draw_bitmap(Image::pupilAtlas[pupilIndex], pupilDrawX, pupilDrawY, 0);
        //}
    }
}
void WorldView::DrawCapturers()
{
    for (size_t i = 0; i < WorldModel::world.mimicGrid.size(); i++)
    {
        if (!Field::field.cellUnderAttack[i])
            continue;

        float drawX = Field::field.cellXYPosition[i].x;
        float drawY = Field::field.cellXYPosition[i].y;
        size_t drawCapturerFrame = Field::field.capturerFrame[i];

        al_draw_bitmap(Image::captureAtlas[drawCapturerFrame], drawX, drawY, 0);
    }
}
void WorldView::DrawStunLightnings()
{
    for (size_t i = 0; i < WorldModel::world.stunLightnings.size(); i++)
    {
        StunLightning *spark = WorldModel::world.stunLightnings[i];

        al_draw_rotated_bitmap(Image::stunLightningAtlas[spark->variant],
                               MimicData::STUN_LIGHTNING_WIDTH / 2, 0,
                               spark->xPosition, spark->yPosition,
                               spark->rotation, spark->isFlipped);
    }
}
void WorldView::DrawRadiation()
{
    for (size_t i = 0; i < WorldModel::world.radiations.size(); i++)
    {
        Radiation *rad = WorldModel::world.radiations[i];

        size_t drawIndex = 2 * rad->largeParticle + rad->blackPolarity;
        float drawX = rad->xPosition;
        float drawY = rad->yPosition;
        float drawCenter = rad->radius;
        float drawSpin = 0.0;

        al_draw_rotated_bitmap(Image::radiationAtlas[drawIndex],
                               drawCenter, drawCenter,
                               drawX, drawY,
                               drawSpin, 0);
    }
}
void WorldView::DrawDialog()
{
    if (Field::field.isStunned)
        al_draw_bitmap(Image::dialogRectPng,
                       Display::width / 2 - FieldData::DIALOG_WIDTH/2 + Field::field.dialogFrameDisplacement.x,
                       Display::height / 2 - FieldData::DIALOG_HEIGHT/2 + Field::field.dialogFrameDisplacement.y,
                       0);
}
