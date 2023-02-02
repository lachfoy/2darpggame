#include "Player.h"

#include "EnemyManager.h"

Player::Player(Vector2 position, Texture texture) : Sprite(position, texture)
{
    mSpeed = 1120.0f;
    mMaxHealth = 100;
    mHealth = mMaxHealth;
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

void Player::Attack()
{
    // option 1 - handle through a decoupled interface
    for (int i = 0; i < mNumObservers; i++) {
        mObservers[i]->OnNotify(*this, 1);
    }

    // option 2 - just leave it coupled 
    EnemyManager::Instance().HandlePlayerAttack(*this);
}

void Player::Update(float deltaTime)
{
    if (mDirection != Vector2::Zero()) {
        mDirection.Normalize();
    }

    // apply speed and friction
    mAcceleration = mDirection * mSpeed;
    mAcceleration -= mVelocity * mFriction;

    // calculate new position and velocity from acceleration
    mPosition += mVelocity * deltaTime + mAcceleration * 0.5f * deltaTime * deltaTime;
    mVelocity += mAcceleration * deltaTime;

    // reset direction
    mDirection = Vector2::Zero();
}

void Player::DrawHealthbar(Renderer& renderer)
{ 

}