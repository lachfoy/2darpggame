#ifndef SPRITE_H_
#define SPRITE_H_

#include "Vector2.h"
#include "Texture.h"
#include "Renderer.h"

class Sprite
{
public:
    Sprite() = default;
    Sprite(Vector2 position, Texture texture);
    ~Sprite() {}

    // getters
    Vector2 Position() const { return mPosition; }

    // setters
    void SetPosition(Vector2 position) { mPosition = position; } 

    // methods
    virtual void Draw(Renderer& renderer);

protected:
    Vector2 mPosition;
    Texture mTexture;

};

#endif