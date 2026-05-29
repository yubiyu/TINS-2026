#pragma once

#include "ui_uibase.h"

#include "core_display.h"

#include "data_tile.h"

#include <string>
#include <unordered_map>

struct TitleMenu : public UIBase
{
    /// Title text
    const std::string TITLE_TEXT = "TINS 2026";
    const int TITLE_TEXT_X = Display::NATIVE_WIDTH/2;
    const int TITLE_TEXT_Y = Tile::HEIGHT * 2;

    const float TITLE_IMAGE_X = 0;
    const float TITLE_IMAGE_Y = 0;

    /// Title options
    enum enumOptions
    {
        OPTION_GAME,
        // OPTION_LOAD_GAME,
        // OPTION_ARCHIVE,
        OPTION_SETTINGS,
        OPTION_EXIT
    };
    static const int FIRST_OPTION = OPTION_GAME;
    static const int LAST_OPTION = OPTION_EXIT;
    static const int NUM_OPTIONS = LAST_OPTION + 1;

    std::array<std::string, NUM_OPTIONS> optionStrings;

    int optionTextX[NUM_OPTIONS] = {};
    const int OPTION_TEXT_X_BASE = TITLE_TEXT_X;

    int optionTextY[NUM_OPTIONS] = {};
    const int OPTION_TEXT_Y_BASE = Tile::HEIGHT * 16;
    const int OPTION_TEXT_Y_SPACING = Tile::HEIGHT * 2.0;

    int targetedOption {};

public:
    static TitleMenu titleMenu;

    /// Title functions
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
    void DoTargetedOption();
};
