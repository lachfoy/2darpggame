#ifndef AABB_H_
#define AABB_H_

#include "Vector2.h"
#include "Renderer.h"

class AABB
{
public:
    AABB() = default;
    //AABB(Vector2 min, Vector2 max);
    AABB(Vector2 center, Vector2 extents);

    Vector2 Min() const { return mMin; }
    Vector2 Max() const { return mMax; }

    bool Intersects(Vector2 other);
    bool Intersects(const AABB& other);
    void Draw(Renderer& renderer);

    Vector2 mMin;
    Vector2 mMax;

};

#endif