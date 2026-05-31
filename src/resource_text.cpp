#include "resource_text.h"
#include "resource_palette.h"

#include <cmath>
#include <iostream>

ALLEGRO_FONT *Text::defaultFontHalfSize;
ALLEGRO_FONT *Text::defaultFont;

ALLEGRO_FONT *Text::fieldTitleFont;
ALLEGRO_FONT *Text::fieldCounterFont;
ALLEGRO_FONT *Text::fieldDialogFont;

void Text::Initialize()
{
    al_init_font_addon();
    al_init_ttf_addon();
    LoadResources();
}

void Text::Uninitialize()
{
    UnloadResources();
    al_shutdown_ttf_addon();
    al_shutdown_font_addon();
}

void Text::LoadResources()
{
    std::cout << "Text: Loading resources." << std::endl;

    defaultFontHalfSize = al_load_ttf_font("PublicPixel.ttf", DEFAULT_FONT_HEIGHT / 2, 0);
    defaultFont = al_load_ttf_font("PublicPixel.ttf", DEFAULT_FONT_HEIGHT, 0);

    fieldTitleFont = al_load_ttf_font("PublicPixel.ttf", FIELD_TITLE_FONT_HEIGHT, 0);
    fieldCounterFont = al_load_ttf_font("PublicPixel.ttf", FIELD_COUNTER_FONT_HEIGHT, 0);
    fieldDialogFont = al_load_ttf_font("PublicPixel.ttf", FIELD_DIALOG_FONT_HEIGHT, 0);
}

void Text::UnloadResources()
{
    std::cout << "Text: Unloading resources." << std::endl;
    al_destroy_font(defaultFontHalfSize);
    al_destroy_font(defaultFont);
}

ALLEGRO_COLOR Text::ColorMarkup(char input)
{
    ALLEGRO_COLOR result = Palette::textDefault;

    switch (input)
    {
    case 'b':
        result = Palette::colours[Palette::COL_BLACK];
        break;
    case 'd':
        result = Palette::colours[Palette::COL_DARK];
        break;
    case 'l':
        result = Palette::colours[Palette::COL_LIGHT];
        break;
    case 'w':
        result = Palette::colours[Palette::COL_WHITE];
        break;
    case 'R':
        result = Palette::debugRed;
        break;
    case 'G':
        result = Palette::debugGreen;
        break;
    case 'B':
        result = Palette::debugBlue;
        break;
    case 'x':
        result = Palette::textDefault;
        break;
    }
    return result;
}

/*
Like al_draw_text, but uses the al_get_glyph_advance API to change colour mid-line when
reading markup such as [b], [w], [x], etc.

Use an escape sequence like "\\[" to actually draw a bracket.

This function returns the ALLEGRO_COLOR used at the last glyph it drawns, in case a subsequent call to
DrawMarkupLine would use it as an initial_colour arg. Useful for multi-line drawing.
*/
ALLEGRO_COLOR Text::DrawMarkupLine(const ALLEGRO_FONT *font, ALLEGRO_COLOR initial_colour, float init_x, float init_y, int alignment_flags, const std::string &line)
{
    int drawX = init_x;
    int drawY = init_y;
    switch (alignment_flags & 0x3) // Borrowing Allegro's align methods. 0x3 masks the lowest two "bits" (actually ints).
    {
    case ALLEGRO_ALIGN_LEFT: // The default. Nothing needs to be done.
        break;

    case ALLEGRO_ALIGN_CENTRE:
        drawX -= al_get_text_width(font, StripMarkupLine(line).c_str()) / 2;
        break;

    case ALLEGRO_ALIGN_RIGHT:
        drawX -= al_get_text_width(font, StripMarkupLine(line).c_str());
        break;
    }
    if (alignment_flags & ALLEGRO_ALIGN_INTEGER)
    {
        drawX = std::floor(drawX);
        drawY = std::floor(drawY);
    }

    ALLEGRO_COLOR glyphColour = initial_colour; // Persists until changed.
    for (size_t i = 0; i < line.length(); i++)
    {
        int codepoint1 = line[i]; // The current character.
        // Handle escape sequence such as "\\[". This is so I can print '[' or ']' in game, which would otherwise be eaten by markup.
        if (codepoint1 == '\\' && i + 1 < line.length())
        {
            int escaped = line[i + 1];
            int next; // I could use the ternary operator here, but I don't like them. Also, ALLEGRO_NO_KERNING is an enumerated type, so I can't mix with a non-enum type in the conditional expression.
            if (i + 2 < line.length())
                next = line[i + 2];
            else
                next = ALLEGRO_NO_KERNING;

            al_draw_glyph(font, glyphColour, drawX, drawY, escaped);
            drawX += al_get_glyph_advance(font, escaped, next);
            i++; // Skip the escaped character.
            continue;
        }

        // The character immediately following the current one. Default value ALLEGRO_NO_KERNING for al_get_glyph_advance in case (i + 1 >= line.length()).
        int codepoint2 = ALLEGRO_NO_KERNING;
        if (i + 1 < line.length())
        {
            codepoint2 = line[i + 1];                // Todo: this assumes that markup such as [b] is always one character long. Can extend.
            if (codepoint1 == Text::colorMarkupOpen) // Look for colour markup character opening char '['.
            {
                size_t closePosition = line.find(Text::colorMarkupClose, i); // Search for a closing ']'.
                if (closePosition != std::string::npos)                      // Found a ']'.
                {
                    glyphColour = Text::ColorMarkup(line[i + 1]);
                    i = closePosition; // Just skip i ahead to closePosition.
                    continue;          // Skip the draw glyph and glyph advance.
                }
            }
        }
        al_draw_glyph(font, glyphColour, drawX, drawY, codepoint1);
        drawX += al_get_glyph_advance(font, codepoint1, codepoint2);
    }
    return glyphColour; // Can be useful when the next line needs to begin with the current colour.
}
/*
Fancy function that calls DrawMarkupLine() as many times as needed
to wrap a long line text into a given max_width.
*/
ALLEGRO_COLOR Text::DrawMarkupMultiline(const ALLEGRO_FONT *font, ALLEGRO_COLOR initial_colour, float init_x, float init_y, float max_width, float line_height, int alignment_flags, std::string &text)
{
    float drawY = init_y;
    ALLEGRO_COLOR glyphColour = initial_colour;

    std::string line;
    std::string lineNoMarkup;

    size_t pos = 0;
    while (pos < text.length()) // Scan the text one character at a time.
    {
        // Handle explicit newlines.
        if (text[pos] == '\n')
        {
            glyphColour = DrawMarkupLine(font, glyphColour, init_x, drawY, alignment_flags, line);
            drawY += line_height;
            line.clear();
            lineNoMarkup.clear();
            pos++;
            continue;
        }

        // Extract next word.
        size_t wordStart = pos;
        while (pos < text.length() && text[pos] != ' ' && text[pos] != '\n')
            ++pos;

        std::string word = text.substr(wordStart, pos - wordStart);
        std::string wordNoMarkup = StripMarkupLine(word);

        // Candidate line.
        std::string testLine = line.empty() 
                                   ? word
                                   : line + " " + word;
        std::string testLineNoMarkup = lineNoMarkup.empty()
                                           ? wordNoMarkup
                                           : lineNoMarkup + " " + wordNoMarkup;

        float testWidth = al_get_text_width(font, testLineNoMarkup.c_str());

        if (testWidth <= max_width || line.empty())
        {
            line = testLine;
            lineNoMarkup = testLineNoMarkup;
        }
        else
        {
            // Flush current line.
            glyphColour = DrawMarkupLine(font, glyphColour, init_x, drawY, alignment_flags, line);
            drawY += line_height;

            // Start new line.
            line = word;
            lineNoMarkup = wordNoMarkup;
        }

        while (pos < text.length() && text[pos] == ' ') // Skip spaces.
            ++pos;
    }

    // Flush last line.
    if (!line.empty())
        glyphColour = DrawMarkupLine(font, glyphColour, init_x, drawY, alignment_flags, line);

    return glyphColour;
}

// Returns a word stripped of colour markup. For use with al_get_line_width().
std::string Text::StripMarkupLine(const std::string &input_word)
{
    std::string result;
    result.reserve(input_word.length()); // Might as well. The word stripped of markup will never be longer than the original word.

    for (size_t i = 0; i < input_word.length(); i++)
    {
        // Handle escape sequences.
        if (input_word[i] == '\\' && i + 1 < input_word.length())
        {
            result += input_word[i + 1]; // Take the escaped character literally.
            i++;                         // Skip the escaped character.
            continue;
        }

        //
        if (input_word[i] == Text::colorMarkupOpen)
        {
            size_t closePosition = input_word.find(colorMarkupClose, i); // Position of next '] following i.
            if (closePosition != std::string::npos)                      // Found a ']'.
            {
                i = closePosition; // Skip iteration ahead to the position of ']'.
                continue;
            }
        }

        // else // Didn't find any ']' following i.
        result += input_word[i];
    }
    return result;
}