#pragma once

/*
Class component used to represent movement between points in the world.
    Part of:
-Camera
-Creature


    Not a part of:
-UIs
-Chunk

*/

#include "data_tile.h"
#include "component_point.h"

#include <cmath>
#include <iostream>

struct Location
{
    Point2Df current {};
    Point2Df destination {};

    bool atDestination{};
    bool approachingDestinationLinear{};
    bool approachingDestinationFractional{};

    static constexpr float APPROACH_DESTINATION_LINEAR_BASELINE_SPEED = 4.0;
    static constexpr float APPROACH_DESTINATION_FRACTIONAL_BASELINE_RATIO = 0.5;
    static constexpr float APPROACH_DESTINATION_TOLERANCE = APPROACH_DESTINATION_LINEAR_BASELINE_SPEED; // Snap to position within range.

    float approachDestinationLinearSpeed = APPROACH_DESTINATION_LINEAR_BASELINE_SPEED;
    float approachDestinationFractionalRatio = APPROACH_DESTINATION_FRACTIONAL_BASELINE_RATIO;

    void Update();
    void SetCurrentXY(float set_x, float set_y);
    void ScrollCurrentXY(float dx, float dy); // Like SetCurrentXY but += dx, d += dy. Convenient for keyboard or mouse control.
    void SetDestinationXY(float set_x, float set_y, bool warp_to_dest);
    void SetCell(int set_x_cell, int set_y_cell);
    void SetDestinationCell(int set_x_cell, int set_y_cell, bool warp_to_dest);

    void ApproachDestinationLinear(float x_change, float y_change);
    void ApproachDestinationFractional(float ratio);
    void WarpToDestination();

    void SetApproachDestinationLinearSpeed(float set_speed) {approachDestinationLinearSpeed = set_speed;}
    void SetApproachDestinationFractionalRatio(float set_ratio) {approachDestinationFractionalRatio = set_ratio;}
};