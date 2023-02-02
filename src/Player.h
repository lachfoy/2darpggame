#ifndef PLAYER_H_
#define PLAYER_H_

#include "Sprite.h"

#include "Vector2.h"
#include "Texture.h"
#include "Renderer.h"
#include "AABB.h"
#include "Observer.h"

class Player : public Sprite
{
public:
    Player(Vector2 position, Texture texture);

    // getters
    int Damage() const { return 20; }
    AABB HitBox() const { return AABB(mPosition, Vector2(10.0f, 10.0f)); }
    AABB AttackHitBox() const { return AABB(mPosition, Vector2(32.0f, 32.0f)); }

    // setters
    void SetDirectionX(float directionX) { mDirection.x = directionX; }
    void SetDirectionY(float directionY) { mDirection.y = directionY; }

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void AddObserver(Observer* observer) { mObservers[mNumObservers] = observer; mNumObservers++; }
    void Attack();
    void Update(float deltaTime);
    void DrawHealthbar(Renderer& renderer);

private:
    Vector2 mDirection;
    Vector2 mVelocity;
    Vector2 mAcceleration;
    float mSpeed;
    int mMaxHealth;
    int mHealth;
    float mFriction = 10.0f;

    // some testing observers code
    Observer* mObservers[1];
    int mNumObservers = 0;

};

#endif