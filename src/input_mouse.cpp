#include "input_mouse.h"

#include <iostream>

bool Mouse::mouseAxesAltered = false;
Point2D Mouse::displayPosition {};
int Mouse::previousZPosition{};
int Mouse::currentZPosition {};

bool Mouse::buttonDown[NUM_BUTTONS] {};
bool Mouse::buttonDownPrevious[NUM_BUTTONS] {};
int Mouse::buttonTicks[NUM_BUTTONS] {};

void Mouse::Initialize()
{
    al_install_mouse();
    // al_hide_mouse_cursor(Display::display);
}

void Mouse::Uninitialize()
{
    al_uninstall_mouse();
}

void Mouse::MouseXYZ(const ALLEGRO_EVENT &event)
{
    mouseAxesAltered = true;

    displayPosition.x = event.mouse.x;
    displayPosition.y = event.mouse.y;
    currentZPosition = event.mouse.z;
}

void Mouse::ButtonDown(const ALLEGRO_EVENT &event)
{
    buttonDown[event.mouse.button] = true;
    //std::cout << "Debug Mouse button clicky: " << event.mouse.button << std::endl;
}

void Mouse::ButtonUp(const ALLEGRO_EVENT &event)
{
    buttonDown[event.mouse.button] = false;
    buttonTicks[event.mouse.button] = 0;
}

void Mouse::Update()
{
    mouseAxesAltered = false;

    previousZPosition = currentZPosition;

    for(size_t i = 0; i < NUM_BUTTONS; i++)
    {
        buttonDownPrevious[i] = buttonDown[i];

        if(buttonDown[i])
            buttonTicks[i] ++;
    }
}