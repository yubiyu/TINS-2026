#include "core_event.h"
#include "core_display.h"
#include "core_timer.h"

ALLEGRO_EVENT_QUEUE *Event::eventQueue;
ALLEGRO_EVENT Event::event;

void Event::Initialize()
{
    eventQueue = al_create_event_queue();

    al_register_event_source(eventQueue, al_get_display_event_source(Display::display));
    al_register_event_source(eventQueue, al_get_timer_event_source(Timer::FPSTimer));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());
}

void Event::Uninitialize()
{
    al_destroy_event_queue(eventQueue);
}
