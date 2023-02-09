#ifndef VECTOR_2_H_
#define VECTOR_2_H_

struct Vector2
{
    Vector2();
    Vector2(float x, float y);

    // operator overloads
    inline Vector2 operator+(const Vector2& rhs) { return Vector2(x + rhs.x, y + rhs.y); }
    inline void operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; }
    inline void operator+=(const float rhs) { x += rhs; y += rhs; }
    inline void operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; }
    inline Vector2 operator-(const Vector2& rhs) { return Vector2(x - rhs.x, y - rhs.y); }
    inline void operator*=(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; }
    inline void operator*=(const float rhs) { x *= rhs; y *= rhs; }
    inline Vector2 operator*(const float rhs) { return Vector2(x * rhs, y * rhs); }
    inline Vector2 operator*(const Vector2& rhs) { return Vector2(x * rhs.x, y * rhs.y); }
    inline void operator/=(const Vector2& rhs) { x /= rhs.x; y /= rhs.y; }
    inline bool operator==(const Vector2& rhs) { return x == rhs.x && y == rhs.y; }
    inline bool operator!=(const Vector2& rhs) { return !(x == rhs.x && y == rhs.y); }

    // getters
    float Length();
    static Vector2 Zero() { return Vector2(0.0f, 0.0f); }

    // methods
    Vector2 Normalize();

    float x;
    float y;

};

#endif