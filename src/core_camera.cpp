#include "core_camera.h"

#include <cmath> // abs, fabs
#include <iostream>

Camera Camera::worldCamera;

void Camera::Initialize()
{
    al_identity_transform(&identityTransform); // Just cache this, so I don't have to reset currentTransform twice every WorldView::Render().

    viewWidth = 0; // Necessary for doing al_translate_transform by a percentage. These need to be set by WorldView::worldView, or whatever this camera belongs to.
    viewHeight = 0;

    location.SetDestinationXY(0, 0, true);

    zoom = 1.0;

    UpdateZoomXYCenter();
    UpdateTransform();

}

void Camera::Uninitialize()
{
}

void Camera::Update()
{
    UpdateLocation(); // Needed in case moving toward a destination.
    // Todo: UpdatePanning() if auto panning toward a destination is implemented.
    // Todo: UpdateZoom() if auto zooming toward a target zoom level is implemented.
}

void Camera::UpdateTransform()
{
    al_identity_transform(&transform);
    
    /*
    Panning
    */
    al_translate_transform(&transform, -location.current.x, -location.current.y);
    /*
    Zooming
    */
    al_translate_transform(&transform, -zoomXCenter, -zoomYCenter); 
    al_scale_transform(&transform, zoom, zoom);
    al_translate_transform(&transform, zoomXCenter, zoomYCenter);
}

void Camera::UpdateLocation()
{
    location.Update();
    UpdateTransform();
}

void Camera::SetXY(float set_x, float set_y)
{
    location.SetCurrentXY(set_x, set_y);
    UpdateTransform();
}

void Camera::ZoomIn()
{
    zoom += ZOOM_STEP;
    if (zoom > ZOOM_MAX)
        zoom = ZOOM_MAX;

    UpdateTransform();
}

void Camera::ZoomOut()
{
    zoom -= ZOOM_STEP;
    if (zoom < ZOOM_MIN)
        zoom = ZOOM_MIN;

    UpdateTransform();
}

void Camera::UpdateZoomXYCenter()
{
    zoomXCenter = viewWidth / 2 + Tile::HALF_WIDTH;
    zoomYCenter = viewHeight / 2 + Tile::HALF_HEIGHT;
    UpdateTransform();
}
void Camera::SetViewDimensions(int view_width, int view_height)
{
    viewWidth = view_width;
    viewHeight = view_height;
    UpdateZoomXYCenter();
}