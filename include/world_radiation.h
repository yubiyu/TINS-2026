#pragma once

struct Radiation
{
    bool isAlive = true;
    bool isShrapenel {};
    int lifespan {};

    float xPosition {}, yPosition {};
    float xVelocity {}, yVelocity {};

    float radius {};

    bool blackPolarity {};
    bool largeParticle {};
    float spinAngle {}, spinChange {};

    void Initialize(float set_x, float set_y, bool set_shrapenel);
    void Update();
};