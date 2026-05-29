#pragma once

#include <random>
#include <string>

/*
Random number engine.
Initialized once and shared between all parts of the program, so that they don't have to instantiate 
new engines whenever random numbers are called for.
*/

struct Random
{
    static unsigned int seed;
    static std::mt19937 mtEngine;

    static void Initialize();
    //static void Uninitialize(){}

    static std::string RandomUUID();

    static int RandomInt(int min, int max);
    static double RandomReal(double min, double max);
    static bool FlipCoin();
    static int RollDice(int dice_rolls, int dice_size);

};