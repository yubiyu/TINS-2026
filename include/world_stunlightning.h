#pragma once

struct StunLightning
{
    bool isAlive = true;

    int lifespan_current {};
    int lifespan_max {};

    float xPosition {}, yPosition {};
    int rotation {};
    int variant {};
    int variantChangeTicks_current {};
    int variantChangeTicks_max {};

    bool isFlipped {};

    void Initialize(float set_x, float set_y);
    void Update();
    void Reroll();

};