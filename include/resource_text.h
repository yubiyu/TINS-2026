#pragma once

#include "data_tile.h"


#include <string>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

struct Text
{
    static ALLEGRO_FONT *defaultFont;
    static ALLEGRO_FONT *defaultFontHalfSize;

    static ALLEGRO_FONT *fieldTitleFont;
    static ALLEGRO_FONT *fieldCounterFont;
    static ALLEGRO_FONT *fieldDialogFont;

    static const int DEFAULT_FONT_HEIGHT = Tile::HEIGHT;
    static const int FIELD_TITLE_FONT_HEIGHT = Tile::HEIGHT*2;
    static const int FIELD_COUNTER_FONT_HEIGHT = Tile::HEIGHT*2;
    static const int FIELD_DIALOG_FONT_HEIGHT = Tile::HEIGHT*2;

    static const char colorMarkupOpen = '['; // Markup is parsed for in blocks such "[b]" for black, or "[w]" for white.
    static const char colorMarkupClose = ']';

    // Borrowing Allegro 5's align methods. See DrawMarkupLine().
    enum enumDrawMarkupLineAlignment : int
    {
        LINE_ALIGN_LEFT = 0,   // bit ...000
        LINE_ALIGN_CENTRE = 1, // bit ...001
        LINE_ALIGN_CENTER = 1, // bit ...010
        LINE_ALIGN_RIGHT = 2,  // bit ...011

        LINE_ALIGN_INTEGER = 4 // Bit ...101
    };

    static void Initialize();
    static void Uninitialize();

    static void LoadResources();
    static void UnloadResources();

    static ALLEGRO_COLOR ColorMarkup(char input);
    static ALLEGRO_COLOR DrawMarkupLine(const ALLEGRO_FONT *font, ALLEGRO_COLOR initial_colour, float init_x, float init_y, int alignment_flags, const std::string &line);
    static ALLEGRO_COLOR DrawMarkupMultiline(const ALLEGRO_FONT *font, ALLEGRO_COLOR initial_colour, float init_x, float init_y, float max_width, float line_height, int alignment_flags, std::string &text);
    static std::string StripMarkupLine(const std::string &input_word);
};
