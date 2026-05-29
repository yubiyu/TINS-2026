#include "ui_titlemenu.h"
#include "ui_settingsmenu.h"

#include "core_uistate.h"
#include "core_display.h"

#include "input_keyboard.h"

#include "resource_image.h"
#include "resource_text.h"
#include "resource_palette.h"

#include "util_text.h"
#include "util_drawing.h"

#include <iostream>

TitleMenu TitleMenu::titleMenu;

void TitleMenu::Initialize()
{
    UIBase::Initialize(UIData::defaultUIRect[UIData::UI_TITLE_MENU]);
    // drawDebugPrimitives = true; // Comment out to disable.

    optionStrings[OPTION_GAME] = "New Game";
    //{OPTION_LOAD_GAME, "Load Game"},
    //{OPTION_ARCHIVE, "Archive"},
    optionStrings[OPTION_SETTINGS] = "Settings";
    optionStrings[OPTION_EXIT] = "Exit";

    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        optionTextX[i] = OPTION_TEXT_X_BASE;
        optionTextY[i] = OPTION_TEXT_Y_BASE + i * OPTION_TEXT_Y_SPACING;
    }

    SetTargetedOption(OPTION_GAME);
}
void TitleMenu::Uninitialize()
{
    UIBase::Uninitialize();
}
void TitleMenu::OnActivation()
{
    if(UIState::gameActive)
        optionStrings[OPTION_GAME] = "Resume Game";
    else
        optionStrings[OPTION_GAME] = "New Game";
}
void TitleMenu::OnDeactivation()
{

}
void TitleMenu::InputMouse()
{
}
void TitleMenu::InputKeyboard()
{
    if (Keyboard::Pressed(ALLEGRO_KEY_UP))
    {
        AdjustTargetedOptionUp();
    }
    else if (Keyboard::Pressed(ALLEGRO_KEY_DOWN))
    {
        AdjustTargetedOptionDown();
    }

    if (Keyboard::Pressed(ALLEGRO_KEY_ESCAPE))
    {
        SetTargetedOption(OPTION_EXIT);
    }
    else if (Keyboard::Pressed(ALLEGRO_KEY_ENTER))
    {
        DoTargetedOption();
    }
}

void TitleMenu::Update()
{
    UpdateBuffer();
}
void TitleMenu::UpdateBuffer()
{
    if (!bufferNeedsUpdate)
        return;
    bufferNeedsUpdate = false;

    ALLEGRO_BITMAP *previousBitmap = al_get_target_bitmap();
    al_set_target_bitmap(buffer);
    al_clear_to_color(Palette::transparent);

    TextUtil::al_draw_string(Text::defaultFont, Palette::textDefault, TITLE_TEXT_X, TITLE_TEXT_Y, ALLEGRO_ALIGN_CENTER, TITLE_TEXT);

    for (int i = 0; i < TitleMenu::NUM_OPTIONS; i++)
    {
        if (targetedOption == i)
        {
            TextUtil::al_draw_string(Text::defaultFont, Palette::textHighlighted,
                                     TitleMenu::optionTextX[i], TitleMenu::optionTextY[i],
                                     ALLEGRO_ALIGN_CENTER, TitleMenu::optionStrings.at(i));
        }
        else
            TextUtil::al_draw_string(Text::defaultFont, Palette::textGreyed,
                                     TitleMenu::optionTextX[i], TitleMenu::optionTextY[i],
                                     ALLEGRO_ALIGN_CENTER, TitleMenu::optionStrings.at(i));
    }

    if (drawDebugPrimitives)
        DrawingUtil::al_draw_inbounds_rectangle(0, 0, width, height, Palette::debugRed, 1.0);

    al_set_target_bitmap(previousBitmap);
}

void TitleMenu::AdjustTargetedOptionUp()
{
    if (targetedOption > FIRST_OPTION)
        targetedOption--;

    bufferNeedsUpdate = true;
}

void TitleMenu::AdjustTargetedOptionDown()
{
    if (targetedOption < LAST_OPTION)
        targetedOption++;

    bufferNeedsUpdate = true;
}

void TitleMenu::SetTargetedOption(int whichOption)
{
    targetedOption = whichOption;

    bufferNeedsUpdate = true;
}

void TitleMenu::DoTargetedOption()
{
    switch (targetedOption)
    {
    case OPTION_GAME:
        UIState::CloseCurrentFullscreenMenu();
        UIState::needsReset = true;
        UIState::gameActive = true;
        break;
    case OPTION_SETTINGS:
        UIState::OpenFullscreenMenu(&SettingsMenu::settingsMenu);
        break;
    case OPTION_EXIT:
        UIState::exit = true;
        break;
    }
    bufferNeedsUpdate = true;
}