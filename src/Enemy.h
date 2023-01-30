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

#include "EnemyState.h"
//class EnemyState;

class Enemy : public Sprite
{
public:
    Enemy(Vector2 position, Player* player, const char* enemyScript);
    ~Enemy();

    bool operator !=(const Enemy& Enemy) const { return (mId != Enemy.Id()); }

    // getters
    int Id() const { return mId; }
    bool Removable() const { return mRemovable; }
    int Damage() const { return mDamage; }

    // setters
    void SetState(EnemyState enemyState) { mState = enemyState; }
    
    // methods
    void AddVelocity(Vector2 velocity) { mVelocity = velocity; }
    void CheckPlayerInRange();
    void TakeDamage(int damage);
    void Heal(int amount);
    void Update(float deltaTime);
    void DrawHealthbar(Renderer& renderer);

    // api
    static int MoveTo(lua_State* L);
    static int Wait(lua_State* L);

private:
    static int GetId()
    {
        static int sId = 0;
        return sId++;
    }

    int mId; // used for equality operator
    bool mRemovable = false;
    Vector2 mVelocity;
    Vector2 mAcceleration;
    float mFriction = 10.0f;
    float mSpeed;
    int mDamage;
    int mMaxHealth;
    int mHealth;
    lua_State* mLuaState;
    //EnemyState* mState;
    //friend class MoveEnemyState;
    //friend class IdleEnemyState;
    EnemyState mState;
    float mRange;
    bool mIsPlayerInRange = false;
    float mColliderRadius;
    Player* mPlayer;

};

#endif