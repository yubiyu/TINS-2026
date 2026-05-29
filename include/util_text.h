#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <allegro5/allegro_font.h>

struct TextUtil
{
    static inline void CapitalizeFirst(std::string &word)
    {
        if (!word.empty())
            word[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(word[0])));
    }

    static inline void PluralizeRegular(std::string &word)
    {
        if (word.ends_with('s') ||
            word.ends_with('x') ||
            word.ends_with('z') ||
            word.ends_with("ch") ||
            word.ends_with("sh"))
            word += "es";
        // Todo: Add if([consonant]y -> ies) to the default rules here.
        // Add list of vowels and a isVowel/isConsonant function to textutil.
        else
            word += 's';
    }

    /*
    Whether to use the indefinite article 'a' or 'an'.
    */
    static inline bool UseAnCondition(std::string &word)
    {
        if (word.empty())
            return false;

        std::string w;
        w.reserve(word.size());
        for (char c : word)
            w += std::tolower(static_cast<unsigned char>(c));

        // Silent h: use 'an'.
        if (w.starts_with("honest") || // honesty
            w.starts_with("hono") ||   // honour, honor
            w.starts_with("hour") ||   // hourglass
            w.starts_with("heir") ||   // heirloom
            w.starts_with("herb"))     // herbalist,
            return true;

        // "you" sound: use 'a'.
        if (w.starts_with("uni") ||
            w.starts_with("use") ||
            w.starts_with("eur"))
            return false;

        // Initialisms such as SQL or : use 'an' if on this list.
        if (std::isupper(static_cast<unsigned char>(word[0])))
        {
            char c = word[0];
            return c == 'A' || c == 'E' || c == 'F' ||
                   c == 'H' || c == 'I' || c == 'L' ||
                   c == 'M' || c == 'N' || c == 'O' ||
                   c == 'R' || c == 'S' || c == 'X';
        }

        // Default vowel rule: use "an".
        char c = w[0];
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
    }

    static inline int al_get_string_width(const ALLEGRO_FONT *f, const std::string &str)
    {
        const char *c = str.c_str();
        return al_get_text_width(f, c);
    }

    static inline void al_draw_string(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, int flags, std::string text)
    {
        const char *c = text.c_str();
        al_draw_text(font, color, x, y, flags, c);
    }

    static inline void al_draw_multiline_string(const ALLEGRO_FONT *font, ALLEGRO_COLOR color, float x, float y, float max_width, float line_height, int flags, std::string &text)
    {
        const char *c = text.c_str();
        al_draw_multiline_text(font, color, x, y, max_width, line_height, flags, c);
    }

    static inline bool do_multiline_line_count_callback(int /*line_num*/, const char * /*line*/, int /*size*/, void *extra)
    {
        ++(*static_cast<int *>(extra)); // increment per-line
        return true;                    // continue processing
    }

    /// Function borrowed from Mark Oates
    static inline int count_multiline_rows(ALLEGRO_FONT *font, float max_width, const std::string &text)
    {
        if (text.empty())
            return 0;

        int multiline_rows = 0;
        al_do_multiline_text(font, max_width, text.c_str(),
                             do_multiline_line_count_callback, /// do_multiline_text_line_num_callback, // Originally used this callback.
                             &multiline_rows);                 // Actually calls the callback function once for each line. Could be many times.

        return multiline_rows;
    }

    struct MultilineWidthCollectorContext
    {
        ALLEGRO_FONT *font;
        std::vector<int> *lineWidths;
    };

    static inline bool collect_multiline_widths_callback(int line_num, const char *line, int size, void *extra)
    {
        (void)line_num;

        MultilineWidthCollectorContext *ctx = static_cast<MultilineWidthCollectorContext *>(extra);
        std::string lineString(line, size);
        ctx->lineWidths->push_back(al_get_text_width(ctx->font, lineString.c_str()));

        return true;
    }

    static inline std::vector<int> collect_multiline_widths(ALLEGRO_FONT *font, float max_width, const std::string &text)
    {
        std::vector<int> lineWidths;

        if (text.empty())
            return lineWidths;

        MultilineWidthCollectorContext ctx{font, &lineWidths};

        al_do_multiline_text(font, max_width, text.c_str(), collect_multiline_widths_callback, &ctx);

        return lineWidths;
    }
};