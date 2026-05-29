#pragma once

#include <allegro5/allegro.h>

struct Keyboard
{
    //static const int INPUT_TAP_TICKS_THRESHOLD = Timer::FPS / 10; // 6 ticks. Used for things like changing pc's facing direction without changing position.

    static bool keyDown[ALLEGRO_KEY_MAX];
    static int keyTicks[ALLEGRO_KEY_MAX];

    static void Initialize();
    static void Uninitialize();

    static void KeyDown(const ALLEGRO_EVENT &ev);
    static void KeyUp(const ALLEGRO_EVENT &ev);

    static void Update(); // The continuous Update() of Keyboard, called every tick.

    // Query functions
    static bool Pressed(int key)  { return keyTicks[key] == 1; }
    static bool Released(int key) { return !keyDown[key]; }     
    static bool Held(int key)     { return keyTicks[key] > 0;}    
    static int  GetHeldTicks(int key) { return keyTicks[key]; }
};
