#include "Player.h"

Player::Player(Vector2 position, GLuint texture) : Sprite(position, texture)
{
    mSpeed = 100.0f;
    mMaxHealth = 100;
    mHealth = mMaxHealth;
    mFriction = 20.0f;
}

void Player::TakeDamage(int damage)
{
    if (mHealth - damage > 0) {
        mHealth -= damage;
    } else {
        mHealth = 0;
        // dead
    }
}

void Player::Heal(int amount)
{
    if (mHealth + amount > mMaxHealth) {
        mHealth = mMaxHealth;
    } else {
        mHealth += amount;
    }
}

void Player::Update(float deltaTime)
{
    if (mAcceleration != Vector2::Zero()) {
        mAcceleration.Normalize();
    }

    // apply speed and friction
    mAcceleration *= mSpeed;
    mAcceleration -= mVelocity * mFriction;

    // calculate new position and velocity from acceleration
    mPosition += mVelocity * deltaTime + mAcceleration * 0.5f * deltaTime * deltaTime;
    mVelocity += mAcceleration * deltaTime;

    // reset acceleration
    mAcceleration = Vector2::Zero();

    std::cout << "vel: " << mVelocity.x << ", " << mVelocity.y << "\n";
}

void Player::DrawHealthbar(Renderer& renderer)
{ 

}