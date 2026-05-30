#include "core.h"

#include "core_timer.h"
#include "core_event.h"
#include "core_display.h"
#include "core_filesystem.h"
#include "core_random.h"
#include "core_uistate.h"
#include "core_camera.h"

#include "input_keyboard.h"
#include "input_mouse.h"

#include "resource_image.h"
#include "resource_text.h"
#include "resource_audio.h"
#include "resource_palette.h"
#include "resource_configuration.h"

#include "data_mimic.h"

#include "world_worldmodel.h"

#include "ui_uibase.h"
#include "ui_worldview.h"

#include "ui_titlemenu.h"
#include "ui_settingsmenu.h"

#include <iostream>

bool Core::redraw;

void Core::MainLoop()
{
    while (!UIState::exit)
    {
        al_wait_for_event(Event::eventQueue, &Event::event);

        if (Event::event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            UIState::exit = true;

        if (Event::event.type == ALLEGRO_EVENT_KEY_DOWN)
            Keyboard::KeyDown(Event::event);
        if (Event::event.type == ALLEGRO_EVENT_KEY_UP)
            Keyboard::KeyUp(Event::event);

        // if(Event::event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY){}
        // if(Event::event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY){}
        if (Event::event.type == ALLEGRO_EVENT_MOUSE_AXES)
            Mouse::MouseXYZ(Event::event);
        if (Event::event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            Mouse::ButtonDown(Event::event);
        if (Event::event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
            Mouse::ButtonUp(Event::event);

        if (Event::event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            Input();
            Update();
            Audio::Update(); // To investigate: Does this mean that audio can only update every 1/60s? Too slow?
            Mouse::Update();
            Keyboard::Update();
        }

        if (redraw && al_is_event_queue_empty(Event::eventQueue))
            Render();
    }
}

bool Core::Initialize(char **argv)
{
    redraw = true;

    al_install_system(ALLEGRO_VERSION_INT, NULL);
    uint32_t allegroVersion = al_get_allegro_version();
    int versionMajor = allegroVersion >> 24;
    int versionMinor = (allegroVersion >> 16) & 255;
    int versionRevision = (allegroVersion >> 8) & 255;
    // int versionRelease = allegroVersion & 255
    int versionRelease = (allegroVersion & 255) - 1; // "The 4th number is 1 + the current patch number." So I'll just subtract 1 and see what happens when a new revision comes out.
    std::cout << "System: Allegro version " << versionMajor << "." << versionMinor << "." << versionRevision << "." << versionRelease << std::endl;

    Display::Initialize();
    Keyboard::Initialize();
    Mouse::Initialize();
    Timer::Initialize();
    Event::Initialize();
    Random::Initialize();
    UIState::Initialize();

    Filesystem::Initialize(argv);
    Configuration::LoadConfigurations();

    UIData::Install();

    MimicData::Install();

    Filesystem::SetPhysfsFileInterface(); // Image, text, and audio resources are stored in data.zip. To be accessed by PHYSfs.
    Palette::Initialize();
    Image::Initialize();
    Text::Initialize();
    Audio::Initialize();
    Filesystem::SetStandardFileInterface();

    WorldModel::world.Initialize();
    Camera::worldCamera.Initialize();


    // Main Rect UIs
    WorldView::worldView.Initialize();

    // Fullscreen Menu UIs
    TitleMenu::titleMenu.Initialize();
    SettingsMenu::settingsMenu.Initialize();
    SettingsMenu::settingsMenu.LoadFromConfig();

    UIState::OpenMainRectUI(&WorldView::worldView);
    UIState::OpenFullscreenMenu(&TitleMenu::titleMenu);

    return true;
}

void Core::Uninitialize()
{
    UIState::Uninitialize();

 

    // Fullscreen Menu UIs
    SettingsMenu::settingsMenu.Uninitialize();
    TitleMenu::titleMenu.Uninitialize();

    // Main Rect UIs
    WorldView::worldView.Uninitialize();

    Camera::worldCamera.Uninitialize();
    WorldModel::world.Uninitialize();

    Audio::Uninitialize();
    Text::Uninitialize();
    Image::Uninitialize();
    Palette::Uninitialize();

    Configuration::UnloadConfigurations();
    Filesystem::Uninitialize();

    Event::Uninitialize();
    Timer::Uninitialize();
    Mouse::Uninitialize();
    Keyboard::Uninitialize();
    Display::Uninitialize();

    al_uninstall_system();
}

void Core::ResetGame()
{
}

void Core::InputMouse()
{
}

void Core::InputKeyboard()
{
    if (Keyboard::Pressed(ALLEGRO_KEY_P))
        Screenshot();
}

void Core::Input()
{
    InputMouse();
    InputKeyboard();

    // Context menu UIs (top priority)
    if (UIState::GetContextMenuIsOpen() && !UIState::onActivationInputBlock)
    {
        UIState::GetCurrentContextMenu()->Input();
        return;
    }
    // Fullscreen Menu UIs
    if (UIState::GetFullscreenMenuIsOpen() && !UIState::onActivationInputBlock)
    {
        UIState::GetCurrentFullscreenMenu()->Input();
        return;
    }

    if (UIState::onActivationInputBlock)
        return;

    if (UIState::GetCurrentMainRectUI())
        UIState::GetCurrentMainRectUI()->Input();

}

void Core::Update()
{
    UIState::Update();

    WorldModel::world.Update();  
    Camera::worldCamera.Update(); 

    // Context menu UIs
    if (UIState::GetContextMenuIsOpen())
        UIState::GetCurrentContextMenu()->Update();
    // Fullscreen Menu UIs
    if (UIState::GetFullscreenMenuIsOpen())
        UIState::GetCurrentFullscreenMenu()->Update();

    // Main Rect UIs
    if (UIState::GetCurrentMainRectUI())
        UIState::GetCurrentMainRectUI()->Update();
    else
        UIState::OpenMainRectUI(&WorldView::worldView);

    // Baseline UIs
}

void Core::Render()
{
    redraw = false;
    al_clear_to_color(Palette::background);

    // Fullscreen Menu UIs
    if (UIState::GetFullscreenMenuIsOpen())
        UIState::GetCurrentFullscreenMenu()->Render();
    else // No main UIs may be drawn when a fullscreen UI is open.
    {
        // Main Rect UIs
        if (UIState::GetCurrentMainRectUI())
            UIState::GetCurrentMainRectUI()->Render();

        // Baseline UIs
    }

    // Context menu UIs
    if (UIState::GetContextMenuIsOpen())
        UIState::GetCurrentContextMenu()->Render();

    al_flip_display();
}

void Core::Screenshot()
{
    ALLEGRO_BITMAP *screenshot;
    screenshot = al_clone_bitmap(al_get_backbuffer(Display::display));
    al_save_bitmap("screenshot.png", screenshot);
    al_destroy_bitmap(screenshot);
    std::cout << "Screenshot taken." << std::endl;
}