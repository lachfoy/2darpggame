#include "Random.h"

Random gRandom;

Random::Random()
{
    gen = std::mt19937(rd()); 
}

int Random::Range(int a, int b)
{
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(gen);
}