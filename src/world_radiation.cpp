#include "world_radiation.h"

#include "core_timer.h"
#include "core_random.h"
#include "core_display.h"

#include <cmath>

#include <allegro5/allegro5.h>

void Radiation::Initialize(float set_x, float set_y, bool set_shrapenel)
{

    xPosition = set_x;
    yPosition = set_y;

    float initSpeed = 0;
    float initAngle = Random::RandomReal(0.0, ALLEGRO_PI*2);
    if (set_shrapenel)
    {
        isShrapenel = true;
        lifespan = Random::RandomInt(Timer::FPS * 1, Timer::FPS * 1);
        radius = 10.0;

        initSpeed = Random::RandomReal(10.0, 20.0);

        spinAngle = initAngle;
        float maxSpinChange = ((ALLEGRO_PI*2)/Timer::FPS) * 2.0;
        spinChange = Random::RandomReal(-maxSpinChange, maxSpinChange);
    }
    else
    {
        initSpeed = Random::RandomReal(10.0, 20.0);

        lifespan = Random::RandomInt(Timer::FPS * 1, Timer::FPS * 15);
        largeParticle = Random::FlipCoin();

        if (largeParticle)
            radius = 2.0;
        else
            radius = 1.5;
    }

    
    xVelocity = initSpeed * std::cos(initAngle);
    yVelocity = initSpeed * std::sin(initAngle);

    blackPolarity = Random::FlipCoin();
}

void Radiation::Update()
{
    lifespan--;
    if (lifespan <= 0)
        isAlive = false;

    xPosition += xVelocity;
    yPosition += yVelocity;

    spinAngle += spinChange;

    if(isShrapenel)
        return; // No bouncy.

    /*
    Bounce off edges of the display.
    */
    if (xPosition - radius < 0)
    {
        xPosition = radius;
        xVelocity = -xVelocity;
    }
    else if (xPosition + radius > Display::width)
    {
        xPosition = Display::width - radius;
        xVelocity = -xVelocity;
    }

    if (yPosition - radius < 0)
    {
        yPosition = radius;
        yVelocity = -yVelocity;
    }
    else if (yPosition + radius > Display::width)
    {
        yPosition = Display::width - radius;
        yVelocity = -yVelocity;
    }
}
