#pragma once

#include "data_ui.h"

#include "component_point.h"

#include <allegro5/allegro.h>
#include <string>

struct UIBase
{
    ALLEGRO_BITMAP *buffer {};
    bool bufferNeedsUpdate {};
    Point2D origin {};
    Point2D endpoint {};
    int width {};
    int height {};

    bool drawDebugPrimitives{};

    UIBase(){}
    ~UIBase(){}

public:
    void Initialize(Rect initial_rect);
    void Uninitialize();

    void Input();
    virtual void InputKeyboard() = 0;
    virtual void InputMouse() = 0;
    bool CheckMouseInUI();
    bool CheckMouseInOtherUI(const UIBase &other_ui);

    virtual void Update() = 0;
    virtual void UpdateBuffer() = 0;
    void ResizeBuffer(int width, int height);

    void Render();

    void UpdatePosition(int origin_x, int origin_y);
    void Activate();
    void Deactivate();
    virtual void OnActivation();
    virtual void OnDeactivation();

    void DrawMenuFrame(const std::string &title_string); // Useful for menus only.
};