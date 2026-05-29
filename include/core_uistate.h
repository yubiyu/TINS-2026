#pragma once

#include "ui_uibase.h"

struct UIState
{
    static bool exit;
    static bool gameActive; // Whether a game is running. No need to run baseline UIs if there is no game.
    static bool needsReset;

    static bool contextMenuIsOpen;
    static UIBase* currentContextMenu;
    static UIBase* previousContextMenu;

    static UIBase* currentMainRectUI;

    static bool fullscreenMenuIsOpen;
    static UIBase* currentFullscreenMenu;

    static bool onActivationInputBlock; // Prevents menus from responding to keypresses or clicks on the tick that another is activated/deactivated.

    static void Initialize();
    static void Uninitialize();

    static void Update();

    static void OpenContextMenu(UIBase *open_menu);
    static void CloseCurrentContextMenu();
    static void ReopenCurrentContextMenu();

    static void OpenMainRectUI(UIBase *open_main_rect_ui);
    static void CloseCurrentMainRectUI();
    static void ReopenCurrentMainRectUI();

    static void OpenFullscreenMenu(UIBase *open_menu);
    static void CloseCurrentFullscreenMenu();
    static void ReopenCurrentFullscreenMenu();

    static bool GetContextMenuIsOpen() {return contextMenuIsOpen;}
    static UIBase* GetCurrentContextMenu() {return currentContextMenu;}

    static UIBase* GetCurrentMainRectUI() {return currentMainRectUI;}

    static bool GetFullscreenMenuIsOpen() { return fullscreenMenuIsOpen;}
    static UIBase* GetCurrentFullscreenMenu() { return currentFullscreenMenu;}
};