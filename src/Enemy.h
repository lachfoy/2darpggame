#ifndef ENEMY_H_
#define ENEMY_H_

#include "Actor.h"

#include "Map.h"
#include "Player.h"
#include <list>
#include <memory>

class Enemy;
typedef std::unique_ptr<Enemy> EnemyPtr;

class Enemy : public Actor
{
public:
    Enemy(int x, int y, const char* enemyScript); // should be just x and y, and some kind of data
    ~Enemy();

    bool operator !=(const Enemy& Enemy) const { return (mId != Enemy.Id()); }

    // getters
    int Id() const { return mId; }

    bool Removable() const { return mRemovable; } // removable???

    // setters
    // ..

    // methods
    void OnDeath() override;
    void Update(Map& map, Player& player, std::list<EnemyPtr>& otherEnemies);

protected:
    int mId; // used for equality operator
    bool mRemovable = false; // assume this starts false
    int mDamage;
    
private:
    static int GetId()
    {
        static int sId = 0;
        return sId++;
    }

};

#endif