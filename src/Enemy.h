#ifndef ENEMY_H_
#define ENEMY_H_

#include "Sprite.h"

#include "Vector2.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Player.h"

class Enemy : public Sprite
{
public:
    Enemy(Vector2 position, const char* enemyScript); // should be just x and y, and some kind of data
    ~Enemy();

    bool operator !=(const Enemy& Enemy) const { return (mId != Enemy.Id()); }

    // getters
    int Id() const { return mId; }
    bool Removable() const { return mRemovable; } // removable???
    int Damage() const { return 20; }

    // setters
    // ..

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void Update(Player& player);
    void DrawHealthbar(Renderer& renderer);

protected:
    int mId; // used for equality operator
    bool mRemovable = false;
    int mDamage;
    int mHealth;
    int mMaxHealth;
    
private:
    static int GetId()
    {
        static int sId = 0;
        return sId++;
    }

};

#endif