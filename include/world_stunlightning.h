#pragma once

struct StunLightning
{
    bool isAlive = true;

    float xPosition {}, yPosition {};
    int variant {};
    int rotation {};

    void Initialize(float set_x, float set_y);
    void Update();

};