#ifndef ACTOR_H_
#define ACTOR_H_

#include "Sprite.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Map.h"

class Actor : public Sprite
{
public:
    Actor(int x, int y, GLuint texture);

    // methods
    bool MoveUp(Map& map);
    bool MoveDown(Map& map);
    bool MoveLeft(Map& map);
    bool MoveRight(Map& map);
    void TakeDamage(int damage);
    virtual void OnDeath() = 0;
    void Heal(int amount);
    void DrawHealthbar(Renderer& renderer);

protected:
    int mHealth;
    int mMaxHealth;
    bool mGreenHealthbar;

};

#endif