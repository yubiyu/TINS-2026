#include "world_mimic.h"

#include "data_mimic.h"

#include "core_random.h"

void Mimic::Initialize(int set_caste)
{
    detonationCD = MimicData::TICKS_TO_DETONATION;

    phasingTicks = MimicData::PHASING_TIME;

    caste = set_caste;
    switch (caste)
    {
    case MimicData::CASTE_MOOK:
        break;

    case MimicData::CASTE_CRAB:
        health = 2;
        break;

    case MimicData::CASTE_REDIRECTOR:
        isRedirector = true;
        redirectionIndex = Random::RandomInt(0, 8);
        pupilShape = MimicData::PUPIL_6;
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
    VaryPupil();
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

    pupilDisplaceTicks_current++;
    if (pupilDisplaceTicks_current >= pupilDisplaceTicks_needed)
    {
        pupilDisplaceTicks_current = 0;
        DisplacePupil();
    }

    pupilVaryTicks_current++;
    if (pupilVaryTicks_current >= pupilVaryTicks_needed)
    {
        pupilVaryTicks_current = 0;
        VaryPupil();
    }
}

void Mimic::DisplacePupil()
{
    pupilDisplaceTicks_needed *= 0.75;
    if (pupilDisplaceTicks_needed < 1)
        pupilDisplaceTicks_needed = 1;

    pupilXDisplacement = Random::RandomReal(-9, 9);
    pupilYDisplacement = Random::RandomReal(-5, 5);
}

void Mimic::VaryPupil()
{
    pupilVariant++;
    if (pupilVariant >= MimicData::NUM_PUPIL_VARIANTS)
        pupilVariant = 0;

    pupilSpriteIndex = pupilShape * MimicData::NUM_PUPIL_VARIANTS + pupilVariant;
}