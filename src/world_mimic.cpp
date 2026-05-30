#include "world_mimic.h"

#include "data_mimic.h"

#include "core_random.h"

void Mimic::Initialize(int set_caste)
{
    phasingTicks = MimicData::PHASING_TIME;

    int caste = set_caste;
    switch(caste)
    {
        case MimicData::CASTE_DRONE:
        break;

        case MimicData::CASTE_ARMOURED:
        health = 2;
        break;

        case MimicData::CASTE_REDIRECTOR:
        isRedirector = true;
        redirectionIndex = Random::RandomInt(0,8);
        break;

        case MimicData::CASTE_EVADER:
        health = 2;
        isEvasive = true;
        break;

        case MimicData::CASTE_STUNNER:
        isStunner = true;
        break;
        
        default:
        break;
    }
}