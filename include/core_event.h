#pragma once

#include <allegro5/allegro.h>

/*
Event is the game's backbone.
At the moment, we only check ALLEGRO_EVENTS.
MainLoop() in "core.h" checks for queued events from timer, keyboard, mouse, etc.
*/

struct Event
{
    static ALLEGRO_EVENT_QUEUE *eventQueue;
    static ALLEGRO_EVENT event;

    static void Initialize();
    static void Uninitialize();
};
