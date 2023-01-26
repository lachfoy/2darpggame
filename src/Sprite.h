#ifndef SPRITE_H_
#define SPRITE_H_

#include "Vector2.h"
#include <glad/glad.h>
#include "Renderer.h"

class Sprite
{
public:
    Sprite() = default;
    Sprite(Vector2 position, GLuint texture);
    ~Sprite() {}

    // getters
    Vector2 Position() const { return mPosition; }

    // setters
    void SetPosition(Vector2 position) { mPosition = position; } 

    // methods
    void Draw(Renderer& renderer);

protected:
    Vector2 mPosition;
    GLuint mTexture;

};

#endif