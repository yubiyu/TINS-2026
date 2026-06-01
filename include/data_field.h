#pragma once

#include <cstddef>
#include <string>
#include <array>

struct FieldData
{
    static constexpr float CELL_WIDTH = 128;
    static constexpr float CELL_HEIGHT = 128;

    static const size_t CAPTURE_ANIMATION_NUM_FRAMES = 8;

    static const int DIALOG_WIDTH = 768;
    static const int DIALOG_HEIGHT = 320;
                                                                
    static const inline std::string title_mimic_suppression_array = "mimic_suppression_array";

    static const inline std::array<std::string, 13> statusTechnobabbleA =
    {
        "core : ",
        "power_grid : ",
        "generator : ",
        "heat_sink : ",
        "sensors : ",
        "cpu : ",
        "comms : ",
        "memory : ",
        "biosphere : ",
        "propulsion : ",
        "shields : ",
        "armour : ",
        "hull : ",
    };

    static const inline std::array<std::string, 4> statusTechnobabbleB = 
    {
        "OK",
        "NORMAL",
        "ERROR",
        "NULL"
    };

    static const inline std::string title_failure_imminent = "array_failure_imminent!";
    static const inline std::string title_containment_breach = "CONTAINMENT_BREACHED";
    static const inline std::array<std::string, 18> randomTitles =
    {
        "dont_turn_around",
        "to_the_moon",
        "you_got_mail",
        "eat_your_veggies",
        "what_is_love",
        "mind_the_gap",
        "the_numbers_mason",
        "skill_issue",
        "resistance_is_futile",
        "there_is_no_spoon",
        "we_remember_earth",
        "i_have_no_mouth",
        "and_i_must_scream",
        "its_a_trap",
        "does_not_compute",
        "walk_without_rhythm",
        "use_the_force",
        ":(){ :|:& };:"
    };

    static const inline std::string dialog_error = "ERROR";
    static const inline std::string dialog_gravimetricinterference = "Gravimetric interference!";
    static const inline std::string dialog_recalibrating = "Recalibrating...";

    

};