#ifndef PLAYER_H_
#define PLAYER_H_

#include "Sprite.h"

#include "Vector2.h"
#include "Texture.h"
#include "Renderer.h"
#include "AABB.h"
#include "Observer.h"
#include "Animation.h"
#include <unordered_map>
#include <pugixml.hpp>

enum class FacingDirection
{
    FACING_SOUTH,
    FACING_EAST,
    FACING_NORTH,
    FACING_WEST
};

class Player : public Sprite
{
public:
    Player(Vector2 position, Texture texture);
    ~Player();

    // getters
    int Damage() const { return 20; }
    AABB HitBox() const { return AABB(mPosition, Vector2(10.0f, 10.0f)); }
    AABB AttackHitBox() const { return AABB(mPosition, Vector2(32.0f, 32.0f)); }

    // setters
    void SetDirectionX(float directionX) { mDirection.x = directionX; }
    void SetDirectionY(float directionY) { mDirection.y = directionY; }
    void SetFacingDirection(FacingDirection facingDirection) { if (facingDirection == mFacingDirection) return; mFacingDirection = facingDirection;}

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void AddObserver(Observer* observer) { mObservers[mNumObservers] = observer; mNumObservers++; }
    void Attack();
    void Update(float deltaTime);
    void Draw(Renderer& renderer) override;
    void DrawHealthbar(Renderer& renderer);

private:
    Vector2 mDirection;
    Vector2 mVelocity;
    Vector2 mAcceleration;
    float mSpeed;
    int mMaxHealth;
    int mHealth;
    float mFriction = 10.0f;

    FacingDirection mFacingDirection = FacingDirection::FACING_SOUTH;
    std::string mCurrentAnimation;
    std::map<std::string, Animation*> mAnimations;

    // some testing observers code
    Observer* mObservers[1];
    int mNumObservers = 0;

};

#endif