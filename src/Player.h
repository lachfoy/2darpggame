#ifndef PLAYER_H_
#define PLAYER_H_

#include "Sprite.h"

#include "Vector2.h"
#include <glad/glad.h>
#include "Renderer.h"

class Player : public Sprite
{
public:
    Player(Vector2 position, GLuint texture);

    // getters
    int Damage() const { return 20; }

    // setters
    void SetDirectionX(float directionX) { mDirection.x = directionX; }
    void SetDirectionY(float directionY) { mDirection.y = directionY; }

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void Update(float deltaTime);
    void DrawHealthbar(Renderer& renderer);

private:
    Vector2 mDirection;
    float mSpeed;
    int mMaxHealth;
    int mHealth;
    
};

#endif