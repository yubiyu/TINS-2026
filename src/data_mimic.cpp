#include "data_mimic.h"

#include "core_timer.h"

int MimicData::PHASING_DISTANCE {};
int MimicData::PHASING_TIME {};
int MimicData::PHASING_SPEED {}; 

void MimicData::Install()
{ 
    PHASING_DISTANCE = 64;
    PHASING_TIME = Timer::FPS * 0.125;
    PHASING_SPEED = PHASING_DISTANCE / PHASING_TIME; 
}