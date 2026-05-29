#include "input_keyboard.h"

#include <iostream>

bool Keyboard::keyDown[ALLEGRO_KEY_MAX] = {};
int Keyboard::keyTicks[ALLEGRO_KEY_MAX] = {};

void Keyboard::Initialize()
{
    al_install_keyboard();
}

void Keyboard::Uninitialize()
{
    al_uninstall_keyboard();
}

void Keyboard::KeyDown(const ALLEGRO_EVENT &ev)
{
    keyDown[ev.keyboard.keycode] = true;
}

void Keyboard::KeyUp(const ALLEGRO_EVENT &ev)
{
    keyDown[ev.keyboard.keycode] = false;
    keyTicks[ev.keyboard.keycode] = 0;
}

void Keyboard::Update()
{
    for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
    {
        if(keyDown[i])
            keyTicks[i] ++;
    }
}

