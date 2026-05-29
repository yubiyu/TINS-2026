#include "ui_settingsmenu.h"
#include "ui_titlemenu.h"

#include "core_uistate.h"
#include "core_display.h"

#include "input_keyboard.h"

#include "resource_image.h"
#include "resource_text.h"
#include "resource_audio.h"
#include "resource_palette.h"
#include "resource_configuration.h"

#include "util_text.h"
#include "util_drawing.h"

#include <iostream>

SettingsMenu SettingsMenu::settingsMenu;

void SettingsMenu::Initialize()
{
    UIBase::Initialize(UIData::defaultUIRect[UIData::UI_SETTINGS_MENU]);

    optionLabels[OPTION_FULLSCREEN] = "Display";
    //{OPTION_RESOLUTION, "Scale"},
    optionLabels[OPTION_VSYNC] = "Vsync";
    optionLabels[OPTION_FPS] = "FPS";
    optionLabels[OPTION_BGM_LEVEL] = "Music";
    optionLabels[OPTION_SFX_LEVEL] = "Sound";
    optionLabels[OPTION_RETURN] = "Return to Title";

    for (size_t i = 0; i < NUM_OPTIONS; i++)
    {
        if (i >= FIRST_DISPLAY_OPTION && i <= LAST_DISPLAY_OPTION)
            optionTextY[i] = OPTION_TEXT_Y_BASE + i * OPTION_TEXT_Y_SPACING + 0 * OPTION_TEXT_Y_SPACING;
        else if (i >= FIRST_AUDIO_OPTION && i <= LAST_AUDIO_OPTION)
            optionTextY[i] = OPTION_TEXT_Y_BASE + i * OPTION_TEXT_Y_SPACING + 1 * OPTION_TEXT_Y_SPACING;
        else if (i == OPTION_RETURN)
            optionTextY[i] = OPTION_TEXT_Y_BASE + i * OPTION_TEXT_Y_SPACING + 2 * OPTION_TEXT_Y_SPACING;

        optionValues[i] = "";
    }

    SetTargetedOption(OPTION_FULLSCREEN);
}
void SettingsMenu::Uninitialize()
{
    UIBase::Uninitialize();
}
void SettingsMenu::OnActivation()
{
    if (UIState::gameActive)
        optionLabels[OPTION_RETURN] = "Return to Game";
    else
        optionLabels[OPTION_RETURN] = "Return to Title";
}
void SettingsMenu::OnDeactivation()
{
}
void SettingsMenu::InputMouse()
{
}

void SettingsMenu::InputKeyboard()
{
    if (Keyboard::Pressed(ALLEGRO_KEY_UP))
    {
        AdjustTargetedOptionUp();
    }
    else if (Keyboard::Pressed(ALLEGRO_KEY_DOWN))
    {
        AdjustTargetedOptionDown();
    }

    if (Keyboard::Pressed(ALLEGRO_KEY_LEFT))
    {
        switch (targetedOption)
        {
        case OPTION_FULLSCREEN:
            // SetFullscreenOption(false);
            break;
            /*
        case OPTION_RESOLUTION:
            SetWindowScaleOption(windowScaleOption - 1);
            break;
            */
        case OPTION_VSYNC:
            SetVsyncOption(vsyncOption - 1);
            break;
        case OPTION_FPS:
            SetFPSOption(fpsOption - 5);
            break;

        case OPTION_BGM_LEVEL:
            SetBgmBarsOption(bgmBars - 1);
            break;
        case OPTION_SFX_LEVEL:
            SetSfxBarsOption(sfxBars - 1);
            break;
        }
    }
    else if (Keyboard::Pressed(ALLEGRO_KEY_RIGHT))
    {
        switch (targetedOption)
        {
        case OPTION_FULLSCREEN:
            // SetFullscreenOption(true);
            break;
            /*
        case OPTION_RESOLUTION:
            SetWindowScaleOption(windowScaleOption + 1);
            break;
            */
        case OPTION_VSYNC:
            SetVsyncOption(vsyncOption + 1);
            break;
        case OPTION_FPS:
            SetFPSOption(fpsOption + 5);
            break;

        case OPTION_BGM_LEVEL:
            SetBgmBarsOption(bgmBars + 1);
            break;
        case OPTION_SFX_LEVEL:
            SetSfxBarsOption(sfxBars + 1);
            break;
        }
    }

    if (Keyboard::Pressed(ALLEGRO_KEY_ESCAPE))
    {
        SetTargetedOption(OPTION_RETURN);
    }
    else if (Keyboard::Pressed(ALLEGRO_KEY_ENTER))
    {
        if (targetedOption == OPTION_RETURN)
        {
            SaveToConfig();

            if (UIState::gameActive)
                UIState::CloseCurrentFullscreenMenu();
            else
                UIState::OpenFullscreenMenu(&TitleMenu::titleMenu);
        }
    }
}

void SettingsMenu::Update()
{
    UpdateBuffer();
}

void SettingsMenu::UpdateBuffer()
{
    if (!bufferNeedsUpdate)
        return;
    bufferNeedsUpdate = false;

    ALLEGRO_BITMAP *previousBitmap = al_get_target_bitmap();
    al_set_target_bitmap(buffer);
    al_clear_to_color(Palette::transparent);

    for (size_t i = 0; i < NUM_OPTIONS; i++)
    {
        if (targetedOption == i)
        {
            TextUtil::al_draw_string(Text::defaultFont, Palette::textHighlighted,
                                     OPTION_LABELS_X, optionTextY[i],
                                     ALLEGRO_ALIGN_LEFT | ALLEGRO_ALIGN_INTEGER, optionLabels.at(i));
            TextUtil::al_draw_string(Text::defaultFont, Palette::textHighlighted,
                                     OPTION_VALUES_X, optionTextY[i],
                                     ALLEGRO_ALIGN_LEFT | ALLEGRO_ALIGN_INTEGER, optionValues.at(i));
        }
        else
        {
            TextUtil::al_draw_string(Text::defaultFont, Palette::textGreyed,
                                     OPTION_LABELS_X, optionTextY[i],
                                     ALLEGRO_ALIGN_LEFT | ALLEGRO_ALIGN_INTEGER, optionLabels.at(i));
            TextUtil::al_draw_string(Text::defaultFont, Palette::textGreyed,
                                     OPTION_VALUES_X, optionTextY[i],
                                     ALLEGRO_ALIGN_LEFT | ALLEGRO_ALIGN_INTEGER, optionValues.at(i));
        }
    }

    al_draw_bitmap(Image::settingsVolumeBarEmptyPng,
                   VOLUME_BARS_X, optionTextY[OPTION_BGM_LEVEL],
                   0);
    al_draw_bitmap_region(Image::settingsVolumeBarFullPng,
                          0, 0,
                          bgmBarsTotalWidth, VOLUME_BAR_HEIGHT,
                          VOLUME_BARS_X, optionTextY[OPTION_BGM_LEVEL],
                          0);
    al_draw_bitmap(Image::settingsVolumeBarEmptyPng,
                   VOLUME_BARS_X, optionTextY[OPTION_SFX_LEVEL],
                   0);
    al_draw_bitmap_region(Image::settingsVolumeBarFullPng,
                          0, 0,
                          sfxBarsTotalWidth, VOLUME_BAR_HEIGHT,
                          VOLUME_BARS_X, optionTextY[OPTION_SFX_LEVEL],
                          0);

    if (drawDebugPrimitives)
        DrawingUtil::al_draw_inbounds_rectangle(0, 0, width, height, Palette::debugRed, 1.0);

    al_set_target_bitmap(previousBitmap);
}

void SettingsMenu::AdjustTargetedOptionUp()
{
    if (targetedOption > FIRST_OPTION)
        targetedOption--;

    bufferNeedsUpdate = true;
}

void SettingsMenu::AdjustTargetedOptionDown()
{
    if (targetedOption < LAST_OPTION)
        targetedOption++;

    bufferNeedsUpdate = true;
}

void SettingsMenu::SetTargetedOption(int whichOption)
{
    targetedOption = whichOption;
    bufferNeedsUpdate = true;
}

/*
void SettingsMenu::UpdateWindowScaleText()
{
    windowWidthText = std::to_string(Display::width);
    windowHeightText = std::to_string(Display::height);

    optionValues[OPTION_RESOLUTION] = windowWidthText + "x" + windowHeightText;
}
*/

void SettingsMenu::SetFullscreenOption(bool is_fullscreen)
{
    fullscreenOption = is_fullscreen;
    Display::SetFullscreen(fullscreenOption);

    if (fullscreenOption)
        optionValues[OPTION_FULLSCREEN] = "[LOCKED]";
    else
        optionValues[OPTION_FULLSCREEN] = "[LOCKED]";

    // UpdateWindowScaleText();
    bufferNeedsUpdate = true;
}

/*
void SettingsMenu::SetWindowScaleOption(int scale)
{
    if (!fullscreenOption)
    {
        windowScaleOption = scale;
        if (windowScaleOption < WINDOW_SCALE_MIN)
            windowScaleOption = WINDOW_SCALE_MIN;
        else if (windowScaleOption > WINDOW_SCALE_MAX)
            windowScaleOption = WINDOW_SCALE_MAX;

        Display::SetWindowScale(windowScaleOption);

        UpdateWindowScaleText();
    }
}
*/

void SettingsMenu::SetVsyncOption(int vsync_mode)
{
    if (vsync_mode >= Display::VSYNC_MODE_FIRST && vsync_mode <= Display::VSYNC_MODE_LAST)
    {
        vsyncOption = vsync_mode;
        Display::SetVsync(vsyncOption);

        if (vsyncOption == Display::VSYNC_DRIVER)
            optionValues[OPTION_VSYNC] = "Driver";
        else if (vsyncOption == Display::VSYNC_ON)
            optionValues[OPTION_VSYNC] = "On";
        else if (vsyncOption == Display::VSYNC_OFF)
            optionValues[OPTION_VSYNC] = "Off";
    }
    bufferNeedsUpdate = true;
}

void SettingsMenu::SetFPSOption(int set_fps)
{
    fpsOption = set_fps;
    if (fpsOption < Display::FPS_MIN)
        fpsOption = Display::FPS_MIN;
    else if (fpsOption > Display::FPS_MAX)
        fpsOption = Display::FPS_MAX;

    Display::SetFPS(fpsOption);

    optionValues[OPTION_FPS] = std::to_string(fpsOption);
    bufferNeedsUpdate = true;
}

void SettingsMenu::SetBgmBarsOption(int bars)
{
    bgmBars = bars;
    if (bgmBars < 0)
        bgmBars = 0;
    else if (bgmBars > MAX_VOLUME_BARS)
        bgmBars = MAX_VOLUME_BARS;

    bgmBarsTotalWidth = VOLUME_BAR_WIDTH * bgmBars;

    Audio::SetBgmGain(bars * 0.1);
    bufferNeedsUpdate = true;
}

void SettingsMenu::SetSfxBarsOption(int bars)
{
    sfxBars = bars;
    if (sfxBars < 0)
        sfxBars = 0;
    else if (sfxBars > MAX_VOLUME_BARS)
        sfxBars = MAX_VOLUME_BARS;

    sfxBarsTotalWidth = VOLUME_BAR_WIDTH * sfxBars;

    Audio::SetSfxGain(bars * 0.1);
    bufferNeedsUpdate = true;
}

void SettingsMenu::SetCheatsEnabledOption(int enabled)
{
    cheatsEnabled = enabled;
    bufferNeedsUpdate = true;
}

void SettingsMenu::SaveToConfig()
{
    Configuration::SetKey(Configuration::settingsCfg, "display", "fullscreen window", fullscreenOption);
    // Configuration::SetKey(Configuration::settingsCfg, "display", "window scale", windowScaleOption);
    Configuration::SetKey(Configuration::settingsCfg, "display", "vsync", vsyncOption);
    Configuration::SetKey(Configuration::settingsCfg, "display", "fps", fpsOption);

    Configuration::SetKey(Configuration::settingsCfg, "audio", "bgm bars", bgmBars);
    Configuration::SetKey(Configuration::settingsCfg, "audio", "sfx bars", sfxBars);

    if (!al_save_config_file("config/settings.cfg", Configuration::settingsCfg))
        std::cout << "Error - SettingsMenu: Failed to save to config/settings." << std::endl;
    else
        std::cout << "SettingsMenu: config/settings.cfg successfully updated." << std::endl;
}

void SettingsMenu::LoadFromConfig()
{
    SetFullscreenOption(Configuration::GetInt(Configuration::settingsCfg, "display", "fullscreen window"));
    // SetWindowScaleOption(Configuration::GetInt(Configuration::settingsCfg, "display", "window scale"));
    SetVsyncOption(Configuration::GetInt(Configuration::settingsCfg, "display", "vsync"));
    SetFPSOption(Configuration::GetInt(Configuration::settingsCfg, "display", "fps"));

    SetBgmBarsOption(Configuration::GetInt(Configuration::settingsCfg, "audio", "bgm bars"));
    SetSfxBarsOption(Configuration::GetInt(Configuration::settingsCfg, "audio", "sfx bars"));

    SetCheatsEnabledOption(Configuration::GetInt(Configuration::settingsCfg, "cheats", "cheats enabled"));
}
