#ifndef VECTOR_2_H_
#define VECTOR_2_H_

struct Vector2
{
    Vector2() = default;
    Vector2(float x, float y);

    // operator overloads
    inline void operator+(const Vector2& rhs) { x += rhs.x; y += rhs.y; }
    inline void operator-(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; }
    inline void operator*(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; }
    inline void operator/(const Vector2& rhs) { x /= rhs.x; y /= rhs.y; }
    inline bool operator==(const Vector2& rhs) { return x == rhs.x && y == rhs.y; }
    inline bool operator!=(const Vector2& rhs) { return !(x == rhs.x && y == rhs.y); }

    // getters
    float Length();

    // methods
    void Normalize();

    float x;
    float y;

};

#endif