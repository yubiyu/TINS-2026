#include "world_mimic.h"

#include "data_mimic.h"

#include "core_random.h"

void Mimic::Initialize(int set_caste)
{
    detonationCD = MimicData::TICKS_TO_DETONATION;

    phasingTicks = MimicData::PHASING_TIME;

    int caste = set_caste;
    switch (caste)
    {
    case MimicData::CASTE_MOOK:
        break;

    case MimicData::CASTE_ARMOURED:
        health = 2;
        break;

    case MimicData::CASTE_REDIRECTOR:
        isRedirector = true;
        redirectionIndex = Random::RandomInt(0, 8);
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

    DisplacePupil();
}

void Mimic::Update()
{
    detonationCD--;
    if (detonationCD <= 0 && !isCaptured)
        isExploding = true;

    if (inPhasing)
    {
        phasingTicks--;
        if (phasingTicks <= 0)
            inPhasing = false;
    }

    frameChangeTicks_current++;
    if (frameChangeTicks_current >= frameChangeTicks_needed)
    {
        frameChangeTicks_current = 0;
        inFrameB = !inFrameB;
    }

    pupilChangeTicks_current++;
    if (pupilChangeTicks_current >= pupilChangeTicks_needed)
    {
        pupilChangeTicks_current = 0;
        DisplacePupil();
    }
}

void Mimic::DisplacePupil()
{
    pupilChangeTicks_needed *= 0.8;
    if (pupilChangeTicks_needed < 1)
        pupilChangeTicks_needed = 1;

    pupilXDisplacement = Random::RandomReal(-9, 9);
    pupilYDisplacement = Random::RandomReal(-5, 5);
}