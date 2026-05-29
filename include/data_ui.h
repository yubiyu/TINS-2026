#pragma once

#include "component_rect.h"

#include <array>

struct UIData
{
    static void Install();
    static void SetDefaultUIRect(int UI, float tile_x, float tile_y, float tile_w, float tile_h);

    /*
    Resizable rectangle regions, in order of precedence.
    Lower ranked regions derive their coords from higher ranked regions.
    */

    static int topRectX, topRectY, topRectW, topRectH;
    static int logRectX, logRectY, logRectW, logRectH;
    static int bottomRectX, bottomRectY, bottomRectW, bottomRectH;
    static int mainRectX, mainRectY, mainRectW, mainRectH;

    /*
    For a UI to be active means it can receive Input(), Update() or Render() in Core.
    */
    enum enumUITypes
    {
        /*Base UIs: all base UIs are active simultaneously, unless a fullscreen UI is active.*/

        /*Main Rect UIs: only be one Main Rect UI can be active at a time. Occupies mainRect coords.*/
        UI_WORLD_VIEW,
      
        /*Fullscreen Menu UIs: only one fullscreen UI can be active at a time. Fully suppresses all other UIs when active.*/
        UI_TITLE_MENU,
        UI_SETTINGS_MENU,
    };
    static const int NUM_UI_TYPES = UI_SETTINGS_MENU + 1;
    static std::array<Rect, NUM_UI_TYPES> defaultUIRect;
};