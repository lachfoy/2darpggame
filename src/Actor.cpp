#include "Actor.h"

//#include <cmath> // sqrt

Actor::Actor(int x, int y, GLuint texture) : Sprite(x, y, texture)
{
}

bool Actor::MoveUp(Map& map)
{
    if (map.IsInWall(mPosX, mPosY - 1)) return false;
    mPosY--;
    return true;
}

bool Actor::MoveDown(Map& map)
{
    if (map.IsInWall(mPosX, mPosY + 1)) return false;
    mPosY++;
    return true;
}

bool Actor::MoveLeft(Map& map)
{
    if (map.IsInWall(mPosX - 1, mPosY)) return false;
    mPosX--;
    return true;
}

bool Actor::MoveRight(Map& map)
{
    if (map.IsInWall(mPosX + 1, mPosY)) return false;
    mPosX++;
    return true;
}

void Actor::TakeDamage(int damage)
{
    if (mHealth - damage > 0) {
        mHealth -= damage;
    } else {
        mHealth = 0;
        OnDeath();
    }
}

void Actor::Heal(int amount)
{
    if (mHealth + amount > mMaxHealth) {
        mHealth = mMaxHealth;
    } else {
        mHealth += amount;
    }
}

void Actor::DrawHealthbar(Renderer& renderer)
{
    int healthBarLength = 32 * (mHealth / static_cast<float>(mMaxHealth));

    if (mGreenHealthbar)
        renderer.DrawRectangle(mPosX, mPosY, healthBarLength, 3, 0.0f, 1.0f, 0.0f, 1.0f);
    else
        renderer.DrawRectangle(mPosX, mPosY, healthBarLength, 3, 1.0f, 0.0f, 0.0f, 1.0f);
}