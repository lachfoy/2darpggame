#ifndef SPRITE_H_
#define SPRITE_H_

#include <glm/glm.hpp>
#include "Renderer.h"

class Sprite
{
public:
    Sprite(int x, int y, GLuint texture);
    ~Sprite() {}

    // getters
    int PosX() const { return mPosX; }
    int PosY() const { return mPosY; }

    // setters
    void SetPosX(int posX) { mPosX = posX; } 
    void SetPosY(int posY) { mPosY = posY; } 

    // methods
    void Draw(Renderer& renderer);

protected:
    int mPosX;
    int mPosY;
    GLuint mTexture;

};

#endif