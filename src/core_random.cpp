#include "core_random.h"

#include <sstream>
#include <iomanip>

unsigned int Random::seed = 0;
std::mt19937 Random::mtEngine{};

void Random::Initialize()
{
    seed = std::random_device{}();
    mtEngine.seed(seed);
}

std::string Random::RandomUUID()
{
    /*
    Generates a Version 4 UUID:
    xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx.
    Check the wikipedia or something.
    */

    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream result;
    result << std::hex;

    for (int i = 0; i < 8; i++)
        result << dis(mtEngine);

    result << "-";

    for (int i = 0; i < 4; i++)
        result << dis(mtEngine);

    result << "-4"; // UUID version 4.

    for (int i = 0; i < 3; i++)
        result << dis(mtEngine);

    result << "-";

    result << dis2(mtEngine); // Variant bits 10xx.

    for (int i = 0; i < 3; i++)
        result << dis(mtEngine);

    result << "-";

    for (int i = 0; i < 12; i++)
        result << dis(mtEngine);

    return result.str();
}

int Random::RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mtEngine);
}

double Random::RandomReal(double min, double max)
{
    std::uniform_real_distribution<double> dist(min, max);
    return dist(mtEngine);
}
bool Random::FlipCoin()
{
    std::bernoulli_distribution dist(0.5);
    return dist(mtEngine);
}
int Random::RollDice(int dice_rolls, int dice_size)
{
    int result = 0;
    std::uniform_int_distribution<int> dist(1, dice_size);
    for (int i = 0; i < dice_rolls; i++)
        result += dist(mtEngine);
    return result;
}