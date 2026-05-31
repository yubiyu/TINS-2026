#pragma once

#include <cstddef>
#include <string>

struct FieldData
{
    static constexpr float CELL_WIDTH = 128;
    static constexpr float CELL_HEIGHT = 128;

    static const size_t CAPTURE_ANIMATION_NUM_FRAMES = 8;

    static const int DIALOG_WIDTH = 768;
    static const int DIALOG_HEIGHT = 320;

    std::string dialog_error = "ERROR";
    std::string dialog_gravimetric_interference = "Gravimetric interference!";
    std::string dialog_recalibrating = "Recalibrating"; // Add ellipses.

    

};