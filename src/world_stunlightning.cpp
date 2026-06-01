#include "world_stunlightning.h"

#include "core_timer.h"
#include "core_random.h"
#include "data_mimic.h"

void StunLightning::Initialize(float set_x, float set_y)
{
    lifespan_max = Timer::FPS * 0.50;
    lifespan_current = lifespan_max;

    xPosition = set_x;
    yPosition = set_y;

    variantChangeTicks_current = 0;
    variantChangeTicks_max = Timer::FPS * 0.05;

    Reroll();
} 

void StunLightning::Update()
{
    lifespan_current --;
    if(lifespan_current <= 0)
        isAlive = false;

    variantChangeTicks_current ++;
    if(variantChangeTicks_current > variantChangeTicks_max)
    {
        variantChangeTicks_current = 0;
        Reroll();
    }
}

void StunLightning::Reroll()
{
    variant = Random::RandomInt(0, MimicData::NUM_STUN_LIGHTNING_VARIANTS - 1);
    rotation = Random::RandomReal(0, ALLEGRO_PI*2);
    isFlipped = Random::FlipCoin();
}