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

bool AABB::Intersects(const AABB& other)
{
    return (
        mMin.x <= other.Max().x &&
        mMax.x >= other.Min().x &&
        mMin.y <= other.Max().y &&
        mMax.y >= other.Min().y
    );
}

void AABB::Draw(Renderer& renderer)
{
    float w = abs(mMax.x - mMin.x);
    float h = abs(mMax.y - mMin.y);
    renderer.DrawRectangle(mMin.x, mMin.y, w, h, 1.0f, 0.0f, 0.0f, 0.5f);
}

