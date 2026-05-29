#pragma once

#include "component_point.h"

#include <allegro5/allegro.h>

struct Mouse
{
    //static const int INPUT_TAP_TICKS_THRESHOLD = Timer::FPS / 10;

    static bool mouseAxesAltered; // Whether MouseXYZ() has been called this cycle. Reset if ALLEGRO_EVENT_MOUSE_AXES was not called.
    static Point2D displayPosition;
    //static int displayXPosition, displayYPosition;
    static int previousZPosition; // the Z coordinate is for the (first) mousewheel.
    static int currentZPosition;


    static constexpr int NUM_BUTTONS = 4; // 0 = filler value that does nothing. 1 = left, 2 = right, 3 = middle. There can be many more buttons, 
    static bool buttonDown[NUM_BUTTONS];
    static bool buttonDownPrevious[NUM_BUTTONS];
    static int buttonTicks[NUM_BUTTONS];

    static void Initialize(); 
    static void Uninitialize();

    static void MouseXYZ(const ALLEGRO_EVENT &event);
    static void ButtonDown(const ALLEGRO_EVENT &event);
    static void ButtonUp(const ALLEGRO_EVENT &event);
    static void Update(); // Continuous logic for mouse. Called every tick.

    // Query functions
    static bool Pressed(int button)      { return buttonDown[button] && !buttonDownPrevious[button]; }
    static bool Released(int button)     { return !buttonDown[button] && buttonDownPrevious[button];}     
    static bool Held(int button)         { return buttonTicks[button] > 0; }    
    static int  GetHeldTicks(int button) { return buttonTicks[button]; }


};
