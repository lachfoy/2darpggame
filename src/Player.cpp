#include "Player.h"

#include <cstdio>
#include <cmath> // sqrt

Player::Player(int x, int y, GLuint texture) : Actor(x, y, texture)
{
    mMaxHealth = 100;
    mHealth = mMaxHealth;
    mGreenHealthbar = true;
}

void Player::Update()
{

}