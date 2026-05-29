#pragma once

#include <allegro5/allegro.h>

/*
Timer governs the number of logic updates per second.

For now, the game scene is redrawn once for each logic update.
Uncapped frame rates and interpolation of drawing positions may change this.
*/

struct Timer
{
    static ALLEGRO_TIMER *FPSTimer;
    static const int FPS = 60;

    static void Initialize();
    static void Uninitialize();
};
