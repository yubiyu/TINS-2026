#include "ui_worldview.h"

#include "core_timer.h"
#include "core_camera.h"
#include "core_uistate.h"

#include "input_mouse.h"
#include "input_keyboard.h"

#include "resource_palette.h"

#include "world_worldmodel.h"

#include "util_drawing.h"

#include <algorithm>

WorldView WorldView::worldView;

void WorldView::Initialize()
{
    UIBase::Initialize(UIData::defaultUIRect[UIData::UI_WORLD_VIEW]);

    drawDebugPrimitives = true;

    Camera::worldCamera.SetViewDimensions(width, height);
    // Set camera x/y speed here.
}

void WorldView::Uninitialize()
{
    UIBase::Uninitialize();
}

void WorldView::Reset()
{
    Uninitialize();
    Initialize();
}

void WorldView::InputMouse()
{
    //if (!CheckMouseInUI())
        //return;

    /*
    if (Mouse::currentZPosition > Mouse::previousZPosition)
        Camera::worldCamera.ZoomIn();
    else if (Mouse::currentZPosition < Mouse::previousZPosition)
        Camera::worldCamera.ZoomOut();
    */

}
void WorldView::InputKeyboard()
{
}
void WorldView::Update()
{
    // Todo: Add conditions for free camera movement as well as centering the camera on other things.
    //CenterCameraOnPC();

    UpdateBuffer();
}
void WorldView::UpdateBuffer()
{
    // Note: The bufferNeedsUpdate checks common to all other UIs are skipped for WorldView since it needs to be true practically every update tick.

    ALLEGRO_BITMAP *previousBitmap = al_get_target_bitmap();
    al_set_target_bitmap(buffer);
    al_clear_to_color(Palette::transparent);

    //al_use_transform(&Camera::worldCamera.transform);



    if (drawDebugPrimitives)
        DrawingUtil::al_draw_inbounds_rectangle(0, 0, width, height, Palette::debugBlue, 1.0);

    //al_use_transform(&Camera::worldCamera.identityTransform);

    al_set_target_bitmap(previousBitmap);
}