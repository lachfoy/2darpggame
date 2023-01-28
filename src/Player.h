#ifndef PLAYER_H_
#define PLAYER_H_

#include "Sprite.h"

#include "Vector2.h"
#include <glad/glad.h>
#include "Renderer.h"

class Player : public Sprite
{
public:
    Player(Vector2 position, GLuint texture);

    // getters
    int Damage() const { return 20; }

    // setters
    void SetAccelerationX(float accelerationX) { mAcceleration.x = accelerationX; }
    void SetAccelerationY(float accelerationY) { mAcceleration.y = accelerationY; }

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void Update(float deltaTime);
    void DrawHealthbar(Renderer& renderer);

private:
    Vector2 mVelocity;
    Vector2 mAcceleration;
    float mSpeed;
    int mMaxHealth;
    int mHealth;
    float mFriction;
    
};

#endif