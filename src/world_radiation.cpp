#include "world_radiation.h"

#include "core_timer.h"
#include "core_random.h"
#include "core_display.h"

#include <cmath>

#include <allegro5/allegro5.h>

void Radiation::Initialize(float set_x, float set_y)
{
    lifespan = Random::RandomInt(Timer::FPS * 1, Timer::FPS * 15);

    xPosition = set_x;
    yPosition = set_y;

    float initSpeed = Random::RandomReal(0.1, 20.0);
    float initAngle = Random::RandomReal(0.0, ALLEGRO_PI);
    xVelocity = initSpeed * std::cos(initAngle);
    yVelocity = initSpeed * std::sin(initAngle);

    blackPolarity = Random::FlipCoin();
    largeParticle = Random::FlipCoin();
    //spinAngle = initAngle;
    //spinChange = Random::RandomReal(0.0, ALLEGRO_PI/Timer::FPS * 2.0);

    if(largeParticle)
        radius = 2.0;
    else
        radius = 1.5;
}

void Radiation::Update()
{
    lifespan --;
    if(lifespan <= 0)
        isAlive = false;

    xPosition += xVelocity;
    yPosition += yVelocity;

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

    //spinAngle += spinChange;
}
