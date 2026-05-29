#pragma once

#include "component_point.h"

struct GeometryUtil
{
    static inline bool
    AABBCollision(float a_x, float a_y, float a_width, float a_height,
                  float b_x, float b_y, float b_width, float b_height)
    {
        if (a_x + a_width <= b_x || b_x + b_width <= a_x)
            return false;
        if (a_y + a_height <= b_y || b_y + b_height <= a_y)
            return false;

        return true;
    }

    static inline float Lerp1Df(float p1, float p2, float t)
    {
        return p1 * (1.0-t) + p2*t;
    }
    static inline Point2Df Lerp2Df(Point2Df p1, Point2Df p2, float t)
    {
        return Point2Df( Lerp1Df(p1.x, p2.x, t), Lerp1Df(p1.y, p2.y, t));
    }
    static inline Point2Df Lerp2Df(float p1_x, float p1_y, float p2_x, float p2_y, float t)
    {
        return Point2Df( Lerp1Df(p1_x, p2_x, t), Lerp1Df(p1_y, p2_y, t));
    }

    /*
    //Old code
    static inline float
    LinearInterpolateX(float x1, float y1, float x2, float y2, float x)
    {
        float y = y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        return y;
    }
    */
};