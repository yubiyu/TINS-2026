#include "component_location.h"

void Location::Update()
{
    if (atDestination)
        return;

    ApproachDestinationLinear(approachDestinationLinearSpeed, approachDestinationLinearSpeed);
}

void Location::SetCurrentXY(float set_x, float set_y)
{
    current.x = set_x;
    current.y = set_y;
}
void Location::ScrollCurrentXY(float dx, float dy)
{
    current.x += dx;
    current.y += dy;
}

void Location::SetDestinationXY(float set_x, float set_y, bool warp_to_dest)
{
    atDestination = false;
    destination.x = set_x;
    destination.y = set_y;

    if (warp_to_dest)
        WarpToDestination();
}

void Location::SetCell(int set_x_cell, int set_y_cell)
{
    SetCurrentXY(set_x_cell * Tile::WIDTH, set_y_cell * Tile::HEIGHT);
}

void Location::SetDestinationCell(int set_x_cell, int set_y_cell, bool warp_to_dest)
{
    SetDestinationXY(set_x_cell * Tile::WIDTH, set_y_cell * Tile::HEIGHT, warp_to_dest);
}

void Location::ApproachDestinationLinear(float x_change, float y_change)
{
    float xPrevious = current.x; // Previous position used to clamp overshoot.
    if (current.x < destination.x)
        current.x += x_change;
    else if (current.x > destination.x)
        current.x -= x_change;
    if ((xPrevious < destination.x && current.x > destination.x) ||
        (xPrevious > destination.x && current.x < destination.x))
        current.x = destination.x;

    float yPrevious = current.y;
    if (current.y < destination.y)
        current.y += y_change;
    else if (current.y > destination.y)
        current.y -= y_change;
    if ((yPrevious < destination.y && current.y > destination.y) ||
        (yPrevious > destination.y && current.y < destination.y))
        current.y = destination.y;

    if (std::fabs(current.x - destination.x) < 0.1f && std::fabs(current.y - destination.y) < 0.01f) // Preferable to fuzzy float matching.
        WarpToDestination();
}

void Location::WarpToDestination()
{
    atDestination = true;
    approachingDestinationLinear = false;
    approachingDestinationFractional = false;

    SetCurrentXY(destination.x, destination.y);
}