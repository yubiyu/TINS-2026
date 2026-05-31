#include "world_mimic.h"
#include "data_mimic.h"

#include "world_field.h"

#include "core_random.h"
#include "core_timer.h"

void Mimic::Initialize(int set_clade)
{
    detonationCD = MimicData::TICKS_TO_DETONATION;

    phasingTicks = MimicData::PHASING_TIME;

    clade = set_clade;
    switch (clade)
    {
    case MimicData::CLADE_MOOK:
        break;

    case MimicData::CLADE_CRAB:
        health = 2;
        shieldRotation = Random::RandomReal(0.0, ALLEGRO_PI);
        shieldRotationRate = (ALLEGRO_PI / Timer::FPS) * 0.5;
        shieldRotatesCCW = Random::FlipCoin();
        break;

    case MimicData::CLADE_REDIRECTOR:
        isRedirector = true;
        break;

    case MimicData::CLADE_SPLITTER:
        isSplitter = true;
        break;

    case MimicData::CLADE_STUNNER:
        isStunner = true;
        pupilShape = MimicData::PUPIL_X;
        break;

    case MimicData::CLADE_VARIABLE:
    {
        int copyRoll = Random::RandomInt(MimicData::CLADE_CRAB, MimicData::CLADE_STUNNER);
        switch (copyRoll)
        {
        case MimicData::CLADE_CRAB:
            health = 2;
            shieldRotation = Random::RandomReal(0.0, ALLEGRO_PI);
            shieldRotationRate = (ALLEGRO_PI / Timer::FPS) * 0.5;
            shieldRotatesCCW = Random::FlipCoin();
            break;
        case MimicData::CLADE_REDIRECTOR:
            isRedirector = true;
            break;
        case MimicData::CLADE_SPLITTER:
            isSplitter = true;
            break;
        case MimicData::CLADE_STUNNER:
            isStunner = true;
            pupilShape = MimicData::PUPIL_X;
            break;
        }
        break;
    }
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

    if (health >= 1)
    {
        if(shieldRotatesCCW)
            shieldRotation -= shieldRotationRate;
        else
            shieldRotation += shieldRotationRate;
    }
}

void Mimic::DisplacePupil()
{
    if (!isStunner)
    {
        pupilDisplaceTicks_needed *= 0.75;
        if (pupilDisplaceTicks_needed < 1)
            pupilDisplaceTicks_needed = 1;
    }

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

void Mimic::SetRedirectionIndex(int set_index)
{
    redirectionIndex = set_index;
    int pupilNumber = Field::indexToKeypad[redirectionIndex];

    pupilShape = MimicData::PUPIL_1 + (pupilNumber - 1); // Honestly this could just be pupilShape = pupilNumber.
}