#ifndef RANDOM_H_
#define RANDOM_H_

// random utility 
#include <random>

class Random
{
public:
    Random();

    int Range(int a, int b);

private:
    std::random_device rd;
    std::mt19937 gen;

};

extern Random gRandom;

#endif