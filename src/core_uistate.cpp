#include "core_uistate.h"

bool UIState::exit{};
bool UIState::gameActive{};
bool UIState::needsReset{};

bool UIState::contextMenuIsOpen{};
UIBase *UIState::currentContextMenu{};
UIBase *UIState::previousContextMenu{};

UIBase *UIState::currentMainRectUI{};

bool UIState::fullscreenMenuIsOpen{};
UIBase *UIState::currentFullscreenMenu{};

bool UIState::onActivationInputBlock{};

void UIState::Initialize()
{
    exit = false;
    needsReset = false;
    gameActive = false;

    contextMenuIsOpen = false;
    currentContextMenu = nullptr;
    previousContextMenu = nullptr;

    currentMainRectUI = nullptr;

    fullscreenMenuIsOpen = false;
    currentFullscreenMenu = nullptr;

    onActivationInputBlock = true;
}
void UIState::Uninitialize()
{
}
void UIState::Update()
{
    onActivationInputBlock = false;
}
void UIState::OpenContextMenu(UIBase *open_menu)
{
    onActivationInputBlock = true;

    previousContextMenu = currentContextMenu;
    CloseCurrentContextMenu();
    contextMenuIsOpen = true; // Don't move this to before CloseCurrentContextMenu();
    currentContextMenu = open_menu;
    currentContextMenu->Activate();
}
void UIState::CloseCurrentContextMenu()
{
    onActivationInputBlock = true;

    contextMenuIsOpen = false;
    if (currentContextMenu)
        currentContextMenu->Deactivate();

    currentContextMenu = nullptr;
}
void UIState::ReopenCurrentContextMenu()
{
    if (!currentContextMenu)
        return;

    UIBase *preservePreviousContextMenu = previousContextMenu;
    OpenContextMenu(currentContextMenu);
    previousContextMenu = preservePreviousContextMenu; // Negate OpenContextMenu's previousContextMenu = currentContextMenu; (i.e. itself).
}
void UIState::OpenMainRectUI(UIBase *open_main_rect_ui)
{
    onActivationInputBlock = true;

    CloseCurrentMainRectUI();
    currentMainRectUI = open_main_rect_ui;
    currentMainRectUI->Activate();
}
void UIState::CloseCurrentMainRectUI()
{
    onActivationInputBlock = true;

    if(currentMainRectUI)
        currentMainRectUI->Deactivate();

    currentMainRectUI = nullptr;
}
void UIState::ReopenCurrentMainRectUI()
{
    if(currentMainRectUI)
        OpenMainRectUI(currentMainRectUI);
}
void UIState::OpenFullscreenMenu(UIBase *open_menu)
{
    onActivationInputBlock = true;

    CloseCurrentFullscreenMenu();
    fullscreenMenuIsOpen = true; // Don't move this to before CloseCurrentFullscreenMenu();
    currentFullscreenMenu = open_menu;
    currentFullscreenMenu->Activate();
}
void UIState::CloseCurrentFullscreenMenu()
{
    onActivationInputBlock = true;

    fullscreenMenuIsOpen = false;
    if (currentFullscreenMenu)
        currentFullscreenMenu->Deactivate();

    currentFullscreenMenu = nullptr;
}
void UIState::ReopenCurrentFullscreenMenu()
{
    if (currentFullscreenMenu)
        OpenFullscreenMenu(currentFullscreenMenu);
}