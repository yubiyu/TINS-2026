#include "data_ui.h"
#include "data_tile.h"

#include "core_display.h"

std::array<Rect, UIData::NUM_UI_TYPES> UIData::defaultUIRect;

void UIData::Install()
{
    SetDefaultUIRect(UI_WORLD_VIEW, 0, 0, Display::widthTiles, Display::heightTiles);

    SetDefaultUIRect(UI_TITLE_MENU, 0, 0, Display::widthTiles, Display::heightTiles);
    SetDefaultUIRect(UI_SETTINGS_MENU, 0, 0, Display::widthTiles, Display::heightTiles);
}

// Args are float for calculating half and quarter tile positions. The final coordinate type is int.
void UIData::SetDefaultUIRect(int id, float tile_x, float tile_y, float tile_w, float tile_h)
{
    defaultUIRect[id].x = tile_x * Tile::WIDTH;
    defaultUIRect[id].y = tile_y * Tile::HEIGHT;
    defaultUIRect[id].w = tile_w * Tile::WIDTH;
    defaultUIRect[id].h = tile_h * Tile::HEIGHT;
}