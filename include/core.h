#pragma once

struct Core
{
    static bool redraw;

    static void MainLoop();

    static bool Initialize(char **argv);
    static void Uninitialize();
    static void ResetGame();

    static void InputMouse();
    static void InputKeyboard();

    static void Input();
    static void Update();
    static void Render();

    static void Screenshot();
};
