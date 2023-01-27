#ifndef ENEMY_H_
#define ENEMY_H_

#include "Sprite.h"

#ifdef __APPLE__
extern "C"
{
    #include <lua/lua.h>
    #include <lua/lauxlib.h>
    #include <lua/lualib.h>
}
#else
extern "C"
{
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}
#endif

#include "Vector2.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Player.h"
#include "EnemyBehaviours.h"
#include <memory>
#include "ConcreteEnemyStates.h"

class EnemyState;

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
    void SetDirection(Vector2 direction) { mDirection = direction; }

    // methods
    void TakeDamage(int damage);
    void Heal(int amount);
    void Move(float deltaTime);
    void Update(float deltaTime);
    void DrawHealthbar(Renderer& renderer);

    // api
    static int MoveTo(lua_State* L);
    static int Wait(lua_State* L);

private:
    int mId; // used for equality operator
    bool mRemovable = false;
    Vector2 mDirection;
    float mSpeed;
    int mDamage;
    int mMaxHealth;
    int mHealth;
    lua_State *mLuaState;
    std::unique_ptr<EnemyBehaviour> mBehaviour;
    std::unique_ptr<EnemyState> mState;
    
    static int GetId()
    {
        static int sId = 0;
        return sId++;
    }

};

#endif