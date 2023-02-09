#include "Vector2.h"

#include <cmath>

Vector2::Vector2()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

float Vector2::Length()
{
    return sqrt(x * x + y * y);
}

Vector2 Vector2::Normalize()
{
    float length = Length();
    x /= length;
    y /= length;
    return *this;
}