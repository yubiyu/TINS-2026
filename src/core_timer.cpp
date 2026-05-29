#include "core_timer.h"

ALLEGRO_TIMER *Timer::FPSTimer;

void Timer::Initialize()
{
    FPSTimer = al_create_timer(1.0 / FPS);
    al_start_timer(FPSTimer);
}

void Timer::Uninitialize()
{
    al_destroy_timer(FPSTimer);
}
