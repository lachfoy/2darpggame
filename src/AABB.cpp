#include "AABB.h"

// AABB::AABB(Vector2 min, Vector2 max) : mMin(min), mMax(max)
// {   
// }

AABB::AABB(Vector2 center, Vector2 extents)
{
    mMin = center - extents;
    mMax = center + extents;
}

bool AABB::Intersects(Vector2 other)
{
    return (
        mMin.x <= other.x &&
        mMax.x >= other.x &&
        mMin.y <= other.y &&
        mMax.y >= other.y
    );
}

bool AABB::Intersects(AABB& other)
{
    return (
        mMin.x <= other.Max().x &&
        mMax.x >= other.Min().x &&
        mMin.y <= other.Max().y &&
        mMax.y >= other.Min().y
    );
}

