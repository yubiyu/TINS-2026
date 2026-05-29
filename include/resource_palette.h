#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>

#include "util_text.h"

#include <string>

struct Palette
{
    enum enumColours
    {
        COL_BLACK,
        COL_DARK,
        COL_LIGHT,
        COL_WHITE,
    };

    static const size_t PALETTE_MARKER_BEGIN = COL_BLACK;
    static const size_t PALETTE_MARKER_END = COL_WHITE;
    static const size_t PALETTE_NUM_COLOURS = PALETTE_MARKER_END + 1;
    static ALLEGRO_COLOR colours[PALETTE_NUM_COLOURS];

    static ALLEGRO_COLOR transparent;
    static ALLEGRO_COLOR background;

    static ALLEGRO_COLOR textDefault;
    static ALLEGRO_COLOR textGreyed;
    static ALLEGRO_COLOR textHighlighted;

    static ALLEGRO_COLOR optionHighlightColour;

    static ALLEGRO_COLOR debugRed;
    static ALLEGRO_COLOR debugGreen;
    static ALLEGRO_COLOR debugBlue;
    static ALLEGRO_COLOR debugYellow;
    static ALLEGRO_COLOR debugMagenta;
    static ALLEGRO_COLOR debugCyan;

    static void Initialize();
    static void Uninitialize();
    static ALLEGRO_COLOR ColourMarkup(char input); // Return the colour associated with markup like b.
};
