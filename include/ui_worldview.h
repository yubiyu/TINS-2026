#pragma once

#include "ui_uibase.h"


#include <memory>

//Camera::worldCamera.SetXY(pcPosition.current.x - width / 2, pcPosition.current.y - height / 2);
class WorldView : public UIBase
{

    const float CAMERA_X_SPEED_DEFAULT = 4.0;
    const float CAMERA_Y_SPEED_DEFAULT = 4.0;
    float cameraXSpeed{CAMERA_X_SPEED_DEFAULT};
    float cameraYSpeed{CAMERA_Y_SPEED_DEFAULT};

public:
    static WorldView worldView;

    void Initialize();
    void Uninitialize();
    void Reset();

    void InputMouse() override;
    void InputKeyboard() override;

    void Update() override;
    void UpdateBuffer() override;

    void DrawGrid();
    void DrawTitle();
    void DrawCounters();
    void DrawPhaseImages();
    void DrawMimics();
    void DrawCapturers();
    void DrawStunLightnings();
    void DrawRadiation();
    void DrawDialog();
};