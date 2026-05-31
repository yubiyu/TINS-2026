#include "world_stunlightning.h"

#include "core_random.h"
#include "data_mimic.h"

void StunLightning::Initialize(float set_x, float set_y)
{
    xPosition = set_x;
    yPosition = set_y;
    variant = Random::RandomInt(0, MimicData::NUM_STUN_LIGHTNING_VARIANTS - 1);
} 

void StunLightning::Update()
{

}