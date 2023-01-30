#ifndef PLAYER_H_
#define PLAYER_H_

#include "Sprite.h"

#include "Vector2.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "AABB.h"
#include "Observer.h"

class Player : public Sprite
{
public:
    Player(Vector2 position, GLuint texture);

    // getters
    int Damage() const { return 20; }
    AABB HitBox() const { return AABB(mPosition, Vector2(1.0f, 1.0f)); }

    // setters
    void SetAccelerationX(float accelerationX) { mAcceleration.x = accelerationX; }
    void SetAccelerationY(float accelerationY) { mAcceleration.y = accelerationY; }

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void AddObserver(Observer* observer) { mObservers[mNumObservers] = observer; mNumObservers++; }
    void Attack();
    void Update(float deltaTime);
    void DrawHealthbar(Renderer& renderer);

private:
    Vector2 mVelocity;
    Vector2 mAcceleration;
    float mSpeed;
    int mMaxHealth;
    int mHealth;
    float mFriction = 10.0f;

    Observer* mObservers[1];
    int mNumObservers = 0;

};

#endif