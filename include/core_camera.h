#pragma once

#include "component_location.h"

#include <allegro5/allegro.h>

/*
Purposes:

1) Offset the drawing positions of world objects using transformations.

2) Save drawing resources.
The game only needs to draw bitmaps that can actually be seen.
Camera essentially determines the boundaries of the visible area, a resizable 2D rectangle.
*/
struct Camera
{
    Location location{};

    int viewWidth {}, viewHeight {};

    /*
    Zoom
    */
    ALLEGRO_TRANSFORM transform; // The sum of all transformations: translate, scale, rotate, invert, etc.
    ALLEGRO_TRANSFORM identityTransform; // Untransformed. Just save a copy of this so I don't have to set up transform from scratch before every Render().

    float zoom {};
    // When zooming in, the camera position doesn't automatically move. Adjust the position by 50% of zoom to compensate.  
    float zoomXCenter {};
    float zoomYCenter {};

    const float ZOOM_STEP = 0.5; 
    const float ZOOM_MIN = 1.0; // Draw scaling at the lowest/furthest zoom magnification.
    const float ZOOM_MAX = 8.0; // Draw scaling at the highest/closest zoom magnification.

public:
    static Camera worldCamera; // Shared camera instance. There can be more than one such instance at a time, for a camera swapping effect.

    void Initialize();
    void Uninitialize();

    void Update();
    void UpdateTransform();
    
    void UpdateLocation();
    void SetXY(float set_x, float set_y);
    void ZoomIn();
    void ZoomOut();
    void UpdateZoomXYCenter();
    void SetViewDimensions(int view_width, int view_height);
};
