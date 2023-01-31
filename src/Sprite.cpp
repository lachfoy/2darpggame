#include "Sprite.h"

Sprite::Sprite(Vector2 position, Texture texture)
{
    mPosition = position;
    mTexture = texture;
}

void Sprite::Draw(Renderer& renderer)
{
    renderer.DrawSprite(mPosition.x, mPosition.y, mTexture);
}