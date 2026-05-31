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

    if(Keyboard::Pressed(ALLEGRO_KEY_PAD_0))
    {
        WorldModel::world.Reset();
        return;
    }

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
        size_t drawClade = phaseImage->clade;
        float drawX = phaseImage->location.current.x - MimicData::SPRITE_WIDTH / 2;
        float drawY = phaseImage->location.current.y - MimicData::SPRITE_HEIGHT / 2;
        al_draw_bitmap(Image::mimicAtlas_phasingMimics[drawClade], drawX, drawY, 0);
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

        size_t mimicClade = mimic->clade;

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
            al_draw_bitmap(Image::mimicAtlas_mimics[mimicClade],
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
            al_draw_bitmap(Image::mimicAtlas_mimicsB[mimicClade], mimicDrawX, mimicDrawY, 0);
        else
            al_draw_bitmap(Image::mimicAtlas_mimicsA[mimicClade], mimicDrawX, mimicDrawY, 0);

        size_t pupilIndex = mimic->pupilSpriteIndex;
        float pupilDrawX = mimicDrawX + mimic->pupilXDisplacement;
        float pupilDrawY = mimicDrawY + mimic->pupilYDisplacement;

        al_draw_bitmap(Image::pupilAtlas[pupilIndex], pupilDrawX, pupilDrawY, 0);

        if(mimic->health > 1)
        {
            al_draw_scaled_rotated_bitmap(Image::bubblePng,
                MimicData::SPRITE_WIDTH/2, MimicData::SPRITE_HEIGHT/2,
                mimic->xPosition, mimic->yPosition,
                2.0, 2.0, 
                mimic->shieldRotation, 0);
        }
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
    {
        al_draw_bitmap(Image::dialogRectPng,
                       Field::field.dialogFrameXY.x + Field::field.dialogFrameDisplacement.x - FieldData::DIALOG_WIDTH/2,
                       Field::field.dialogFrameXY.y + Field::field.dialogFrameDisplacement.y - FieldData::DIALOG_HEIGHT/2,
                       0);

        TextUtil::al_draw_string(Text::fieldDialogFont, Palette::colours[Palette::COL_WHITE],
                                 Field::field.dialogErrorXY.x + Field::field.dialogErrorDisplacement.x,
                                 Field::field.dialogErrorXY.y + Field::field.dialogErrorDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2 - 4, // Backdrop,
                                 ALLEGRO_ALIGN_CENTER, FieldData::dialog_error);
        TextUtil::al_draw_string(Text::fieldDialogFont, Palette::colours[Palette::COL_BLACK],
                                 Field::field.dialogErrorXY.x + Field::field.dialogErrorDisplacement.x,
                                 Field::field.dialogErrorXY.y + Field::field.dialogErrorDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2,
                                 ALLEGRO_ALIGN_CENTER, FieldData::dialog_error);

        TextUtil::al_draw_string(Text::fieldDialogFont, Palette::colours[Palette::COL_WHITE],
                                 Field::field.dialogGravimetricInterferenceXY.x + Field::field.dialogGravimetricInterferenceDisplacement.x,
                                 Field::field.dialogGravimetricInterferenceXY.y + Field::field.dialogGravimetricInterferenceDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2 - 4, // Backdrop
                                 ALLEGRO_ALIGN_CENTER, FieldData::dialog_gravimetric_interference);
        TextUtil::al_draw_string(Text::fieldDialogFont, Palette::colours[Palette::COL_BLACK],
                                 Field::field.dialogGravimetricInterferenceXY.x + Field::field.dialogGravimetricInterferenceDisplacement.x,
                                 Field::field.dialogGravimetricInterferenceXY.y + Field::field.dialogGravimetricInterferenceDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2,
                                 ALLEGRO_ALIGN_CENTER, FieldData::dialog_gravimetric_interference);

        float recalibrationBarMaxWidth = 576;
        float stunRecoveryPercent =  static_cast<float>(Field::field.stunRecovery_current)/Field::field.stunRecovery_Max; 
        float recalibrationBarCurrentWidth = recalibrationBarMaxWidth * stunRecoveryPercent;

        float x1Outline = Field::field.dialogRecalibratingXY.x + Field::field.dialogRecalibratingDisplacement.x - recalibrationBarMaxWidth/2;
        float x1Fill = Field::field.dialogRecalibratingXY.x + Field::field.dialogRecalibratingDisplacement.x - recalibrationBarCurrentWidth/2;
        float y1 = Field::field.dialogRecalibratingXY.y + Field::field.dialogRecalibratingDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2 - 4;
        float x2Outline = Field::field.dialogRecalibratingXY.x + Field::field.dialogRecalibratingDisplacement.x + recalibrationBarMaxWidth/2;
        float x2Fill = Field::field.dialogRecalibratingXY.x + Field::field.dialogRecalibratingDisplacement.x + recalibrationBarCurrentWidth/2;
        float y2 = Field::field.dialogRecalibratingXY.y + Field::field.dialogRecalibratingDisplacement.y + 20;

        al_draw_filled_rectangle(x1Fill, y1, x2Fill, y2, Palette::colours[Palette::COL_LIGHT]);
        DrawingUtil::al_draw_inbounds_rectangle(x1Outline, y1, x2Outline, y2, Palette::colours[Palette::COL_DARK], 4.0);

        TextUtil::al_draw_string(Text::fieldDialogFont, Palette::colours[Palette::COL_WHITE],
                                 Field::field.dialogRecalibratingXY.x + Field::field.dialogRecalibratingDisplacement.x,
                                 Field::field.dialogRecalibratingXY.y + Field::field.dialogRecalibratingDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2 - 4, // Backdrop
                                 ALLEGRO_ALIGN_CENTER, FieldData::dialog_recalibrating);
        TextUtil::al_draw_string(Text::fieldDialogFont, Palette::colours[Palette::COL_BLACK],
                                 Field::field.dialogRecalibratingXY.x + Field::field.dialogRecalibratingDisplacement.x,
                                 Field::field.dialogRecalibratingXY.y + Field::field.dialogRecalibratingDisplacement.y - Text::FIELD_DIALOG_FONT_HEIGHT / 2,
                                 ALLEGRO_ALIGN_CENTER, FieldData::dialog_recalibrating);
    }
}
