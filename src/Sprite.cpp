#include "Sprite.h"

Sprite::Sprite(int x, int y, GLuint texture)
{
    mPosX = x;
    mPosY = y;
    mTexture = texture;
}

void Sprite::Draw(Renderer& renderer)
{
    renderer.DrawSprite(mPosX, mPosY, mTexture);
}