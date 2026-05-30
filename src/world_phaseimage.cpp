#include "world_phaseimage.h"

#include "data_mimic.h"

void PhaseImage::Initialize(float set_x, float set_y, float set_x_dest, float set_y_dest)
{
    location.SetCurrentXY(set_x, set_y);
    location.SetDestinationXY(set_x_dest, set_y_dest, false);
    location.SetApproachDestinationLinearSpeed(MimicData::PHASING_SPEED);
}