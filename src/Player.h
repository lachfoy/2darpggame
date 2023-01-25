#ifndef PLAYER_H_
#define PLAYER_H_

#include "Actor.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Map.h"

class Player : public Actor
{
public:
    Player(int x, int y, GLuint texture);

    int GetDamage() const { return 20; }

    // methods
    void OnDeath() override {} // do nothing atm
    void Update();

    void Hi() { std::cout << "hey!!!\n"; }

private:


};

#endif