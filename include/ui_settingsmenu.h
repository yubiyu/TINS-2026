#pragma once

#include "ui_uibase.h"

#include "data_tile.h"

#include <array>
#include <string>


struct SettingsMenu : public UIBase
{
    /// Options
    enum enumOptions : size_t
    {
        OPTION_FULLSCREEN = 0,
        //OPTION_RESOLUTION = 1,
        OPTION_VSYNC = 1,
        OPTION_FPS = 2,

        OPTION_BGM_LEVEL = 3,
        OPTION_SFX_LEVEL = 4,
        OPTION_RETURN = 5
    };
    static const size_t FIRST_OPTION = OPTION_FULLSCREEN;
    static const size_t LAST_OPTION = OPTION_RETURN;
    static const size_t NUM_OPTIONS = LAST_OPTION + 1;
    static const size_t FIRST_DISPLAY_OPTION = OPTION_FULLSCREEN;
    static const size_t LAST_DISPLAY_OPTION = OPTION_FPS;
    static const size_t FIRST_AUDIO_OPTION = OPTION_BGM_LEVEL;
    static const size_t LAST_AUDIO_OPTION = OPTION_SFX_LEVEL;

    std::array<std::string, NUM_OPTIONS> optionLabels{};
    std::array<std::string, NUM_OPTIONS> optionValues {};

    const int OPTION_LABELS_X = Tile::WIDTH * 1;
    const int OPTION_VALUES_X = Tile::WIDTH * 9;

    int optionTextY[NUM_OPTIONS] {};
    const int OPTION_TEXT_Y_BASE = Tile::HEIGHT * 1;
    const int OPTION_TEXT_Y_SPACING = Tile::HEIGHT * 2.0;

    size_t targetedOption {};

    /// Graphics settings
    bool fullscreenOption {};
    int windowScaleOption {};
    const int WINDOW_SCALE_MIN = 1;
    const int WINDOW_SCALE_MAX = 10;
    std::string windowWidthText {};
    std::string windowHeightText {};
    int vsyncOption {};
    int fpsOption {};

    /// Audio settings
    int bgmBars {};
    int sfxBars {};
    const int MAX_VOLUME_BARS = 10;
    const float VOLUME_BARS_X = OPTION_VALUES_X;
    const float VOLUME_BAR_WIDTH = Tile::WIDTH;
    float bgmBarsTotalWidth {};
    float sfxBarsTotalWidth {};
    const float VOLUME_BAR_HEIGHT = Tile::HEIGHT;

    /// Cheat settings
    bool cheatsEnabled {};

public:
    static SettingsMenu settingsMenu;

    /// Settings functions
    void Initialize();
    void Uninitialize();
    void OnActivation() override;
    void OnDeactivation() override;

    void InputMouse() override;
    void InputKeyboard() override;

    void Update() override;
    void UpdateBuffer() override;

    void AdjustTargetedOptionUp();
    void AdjustTargetedOptionDown();
    void SetTargetedOption(int whichOption);

    void UpdateWindowScaleText();

    void SetFullscreenOption(bool is_fullscreen);
    //static void SetWindowScaleOption(int scale);
    void SetVsyncOption(int vsync_mode);
    void SetFPSOption(int set_fps);

    void SetBgmBarsOption(int bars);
    void SetSfxBarsOption(int bars);

    void SetCheatsEnabledOption(int enabled);

    void SaveToConfig();
    void LoadFromConfig();
};
