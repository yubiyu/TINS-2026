#pragma once

#include <allegro5/allegro_primitives.h>

struct DrawingUtil
{
    // Internal thickness
    static inline void al_draw_inbounds_rectangle(float x1, float y1, float x2, float y2, ALLEGRO_COLOR color, float thickness)
    {
        al_draw_rectangle(x1 + thickness / 2, y1 + thickness / 2, x2 - thickness / 2, y2 - thickness / 2, color, thickness);
    }

    static inline void
    al_round_rgb(ALLEGRO_COLOR *color)
    {
        int ri = static_cast<int>(color->r * 255.0f + 0.5f);
        int gi = static_cast<int>(color->g * 255.0f + 0.5f);
        int bi = static_cast<int>(color->b * 255.0f + 0.5f);

        *color = al_map_rgb(ri, gi, bi);
    }
};