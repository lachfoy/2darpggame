#include "Player.h"

Player::Player(Vector2 position, GLuint texture) : Sprite(position, texture)
{
    mSpeed = 3.0f;
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

void Player::Update(float deltaTime)
{
    if (mDirection != Vector2::Zero()) {
        mDirection.Normalize();
        mPosition.x += mDirection.x * mSpeed * deltaTime;
        mPosition.y += mDirection.y * mSpeed * deltaTime;
    }
}

void Player::DrawHealthbar(Renderer& renderer)
{

}