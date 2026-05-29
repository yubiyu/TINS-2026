#include "ui_uibase.h"

#include "input_keyboard.h"
#include "input_mouse.h"

#include "resource_palette.h"
#include "resource_image.h"
#include "resource_text.h"

#include "util_drawing.h"

void UIBase::Initialize(Rect init_rect)
{
    origin.x = init_rect.x;
    origin.y = init_rect.y;
    width = init_rect.w;
    height = init_rect.h;
    endpoint.x = origin.x + width;
    endpoint.y = origin.y + height;

    buffer = al_create_bitmap(width, height);
    bufferNeedsUpdate = true;

    drawDebugPrimitives = false;
}

void UIBase::Uninitialize()
{
    al_destroy_bitmap(buffer);
}

void UIBase::Input()
{
    InputMouse(); // Supposed to use the overridden definitions in child UI classes.
    InputKeyboard();
}

void UIBase::Update()
{
    // Nothing here. This function is meant to be overriden.
}

bool UIBase::CheckMouseInUI()
{
    int mouseX = Mouse::displayPosition.x;
    int mouseY = Mouse::displayPosition.y;
    return (mouseX >= origin.x && mouseX < endpoint.x && mouseY >= origin.y && mouseY < endpoint.y);
}

bool UIBase::CheckMouseInOtherUI(const UIBase &other_ui)
{
    int mouseX = Mouse::displayPosition.x;
    int mouseY = Mouse::displayPosition.y;
    return (mouseX >= other_ui.origin.x && mouseX < other_ui.endpoint.x &&
            mouseY >= other_ui.origin.y && mouseY < other_ui.endpoint.y);
}

void UIBase::UpdateBuffer()
{
}
void UIBase::ResizeBuffer(int width, int height)
{
    al_destroy_bitmap(buffer);
    buffer = al_create_bitmap(width, height);

    bufferNeedsUpdate = true;
}

void UIBase::Render()
{
    al_draw_bitmap(buffer, origin.x, origin.y, 0);
}

void UIBase::UpdatePosition(int origin_x, int origin_y)
{
    origin.x = origin_x;
    origin.y = origin_y;
    endpoint.x = origin.x + width;
    endpoint.y = origin.y + height;
}

void UIBase::Activate()
{
    bufferNeedsUpdate = true;
    OnActivation();
}
void UIBase::Deactivate()
{
    ALLEGRO_BITMAP *previousBitmap = al_get_target_bitmap();
    al_set_target_bitmap(buffer);
    al_clear_to_color(Palette::transparent);
    al_set_target_bitmap(previousBitmap);
    OnDeactivation();
}
void UIBase::OnActivation()
{
    // Nothing here. This virtual function is meant to be overriden.
}
void UIBase::OnDeactivation()
{
    // Nothing here. This virtual function is meant to be overriden.
}