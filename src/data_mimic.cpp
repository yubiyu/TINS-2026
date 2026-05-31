#include "data_mimic.h"

#include "core_timer.h"

std::array<int, MimicData::NUM_CLADES> MimicData::progressToEncounterClade {};

int MimicData::PHASING_DISTANCE {};
int MimicData::PHASING_TIME {};
int MimicData::PHASING_SPEED {}; 

int MimicData::TICKS_TO_DETONATION {};

void MimicData::Install()
{ 
    progressToEncounterClade[CLADE_MOOK] = 0;
    progressToEncounterClade[CLADE_CRAB] = 10;
    progressToEncounterClade[CLADE_REDIRECTOR] = 20;
    progressToEncounterClade[CLADE_SPLITTER] = 60;
    progressToEncounterClade[CLADE_STUNNER] = 75;
    progressToEncounterClade[CLADE_VARIABLE] = 150;

    PHASING_DISTANCE = 64;
    PHASING_TIME = Timer::FPS * 0.175;
    PHASING_SPEED = PHASING_DISTANCE / PHASING_TIME; 

    TICKS_TO_DETONATION = Timer::FPS * 4.0;

}