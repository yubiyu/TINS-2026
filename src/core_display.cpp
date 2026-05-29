#include "core_display.h"

#include "data_tile.h"

#include <iostream>

ALLEGRO_MONITOR_INFO Display::monitorInfo;
ALLEGRO_DISPLAY *Display::display;
// ALLEGRO_BITMAP *Display::scaleBuffer;

// int Display::windowScale;
int Display::width;
int Display::height;
int Display::widthTiles;
int Display::heightTiles;

int Display::fps;

void Display::Initialize()
{
    display = al_create_display(NATIVE_WIDTH, NATIVE_HEIGHT);
    if (!display)
    {
        std::cerr << "Display: Failed to create display!\n";
        return;
    }
    width = al_get_display_width(display);
    height = al_get_display_height(display);
    widthTiles = width / Tile::WIDTH;
    heightTiles = height / Tile::HEIGHT;

    // scaleBuffer = al_create_bitmap(NATIVE_WIDTH, NATIVE_HEIGHT);

    // SetWindowScale(1);

    al_get_monitor_info(al_get_display_adapter(display), &monitorInfo);
    al_set_window_position(display, monitorInfo.x2 / 2 - al_get_display_width(display) / 2, monitorInfo.y2 / 2 - al_get_display_height(display) / 2);
}

void Display::Uninitialize()
{
    // al_destroy_bitmap(scaleBuffer);
    al_set_display_flag(display, ALLEGRO_FRAMELESS, true);
    al_destroy_display(display);
}

/*
void Display::Resize()
{
    al_resize_display(display, width, height);

    al_get_monitor_info(al_get_display_adapter(display), &monitorInfo);
    al_set_window_position(display, monitorInfo.x2 / 2 - al_get_display_width(display) / 2, monitorInfo.y2 / 2 - al_get_display_height(display) / 2);

    //al_destroy_bitmap(scaleBuffer);
    //scaleBuffer = al_create_bitmap(width, height);
}
*/

void Display::SetFullscreen(bool is_fullscreen)
{
    al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, is_fullscreen);

    width = al_get_display_width(display);
    height = al_get_display_height(display);
    widthTiles = width / Tile::WIDTH;
    heightTiles = height / Tile::HEIGHT;

    // Resize();
}

/*
void Display::SetWindowScale(int scale)
{
    windowScale = scale;

    if (al_get_display_flags(display) & ALLEGRO_FULLSCREEN_WINDOW)
    {
        width = al_get_display_width(display);
        height = al_get_display_height(display);
    }
    else
    {
        width = NATIVE_WIDTH * windowScale;
        height = NATIVE_HEIGHT * windowScale;
    }

    Resize();
}
*/

void Display::SetVsync(int vsync_mode)
{
    if (vsync_mode >= VSYNC_MODE_FIRST && vsync_mode <= VSYNC_MODE_LAST)
    {
        al_set_display_option(display, ALLEGRO_VSYNC, vsync_mode);
        std::cout << "Display: vsync mode set to " << al_get_display_option(display, ALLEGRO_VSYNC) << "." << std::endl;
    }
}

void Display::SetFPS(int set_fps)
{
    if (set_fps >= FPS_MIN && set_fps <= FPS_MAX)
    {
        fps = set_fps;
        std::cout << "Display: FPS set to " << set_fps << "." << std::endl;
    }
}
