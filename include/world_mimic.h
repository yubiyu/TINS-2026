#pragma once

#include <cstddef>

class Mimic
{
    int caste {};

    int health = 1;
    int ticksToAttack = 60 * 4;

    bool isRedirector = false;
    size_t redirectionIndex  = 0;

    bool isEvasive = false;

    bool isStunner = false;


public:
    void Initialize(int set_caste);



};