#include "resource_palette.h"

#include <iostream>

ALLEGRO_COLOR Palette::colours[PALETTE_NUM_COLOURS];

ALLEGRO_COLOR Palette::transparent;
ALLEGRO_COLOR Palette::background;

ALLEGRO_COLOR Palette::textDefault;
ALLEGRO_COLOR Palette::textHighlighted;
ALLEGRO_COLOR Palette::textGreyed;

ALLEGRO_COLOR Palette::optionHighlightColour;

ALLEGRO_COLOR Palette::debugRed;
ALLEGRO_COLOR Palette::debugGreen;
ALLEGRO_COLOR Palette::debugBlue;
ALLEGRO_COLOR Palette::debugYellow;
ALLEGRO_COLOR Palette::debugMagenta;
ALLEGRO_COLOR Palette::debugCyan;

void Palette::Initialize()
{
    colours[COL_BLACK] = al_map_rgb(0x20, 0x20, 0x20);
    colours[COL_DARK] = al_map_rgb(0x60, 0x60, 0x60);
    colours[COL_LIGHT] = al_map_rgb(0x9f, 0x9f, 0x9f);
    colours[COL_WHITE] = al_map_rgb(0xdf, 0xdf, 0xdf);

    transparent = al_map_rgba(0, 0, 0, 0);
    background = colours[COL_WHITE];

    textDefault = colours[COL_DARK];
    textGreyed = colours[COL_LIGHT];
    textHighlighted = colours[COL_BLACK];

    ALLEGRO_COLOR oHlCol = colours[COL_LIGHT];
    optionHighlightColour = al_premul_rgba(255 * oHlCol.r, 255 * oHlCol.g, 255 * oHlCol.b, 255 - 64);

    debugRed = al_map_rgb(255, 0, 0);
    debugGreen = al_map_rgb(0, 255, 0);
    debugBlue = al_map_rgb(0, 0, 255);
    debugYellow = al_map_rgb(255, 255, 0);
    debugMagenta = al_map_rgb(255, 0, 255);
    debugCyan = al_map_rgb(0, 255, 255);

    
}

void Palette::Uninitialize()
{
}