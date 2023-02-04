#include "Enemy.h"

#include "Random.h"

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

#include "TextureManager.h"

Enemy::Enemy(Vector2 position, Player* player, const char* enemyScript)
{
    mPosition = position;
    mPlayer = player;

    // open Lua file
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "MoveTo", MoveTo); // expose api function to lua

    // run the lua file
    if (luaL_dofile(L, enemyScript) != 0) {
        std::cout << lua_tostring(L, -1) << "\n"; // error at top of stack
    }

    // query values 
    lua_getglobal(L, "texture");
    lua_getglobal(L, "speed");
    lua_getglobal(L, "damage");
    lua_getglobal(L, "max_health");

    // get values from the stack
    mTexture = TextureManager::Instance().GetTexture(lua_tostring(L, -4));
    mSpeed = lua_tonumber(L, -3);
    mDamage = lua_tonumber(L, -2);
    mMaxHealth = lua_tonumber(L, -1);
    mHealth = mMaxHealth;

    // close lua
    lua_close(L);

    // set default state to idle
    mState = EnemyState::STATE_IDLE;

    mDetectRange = 64.0f;

    mId = GetId();
}

Enemy::~Enemy()
{
}

void Enemy::CheckPlayerInRange()
{
    if (!mPlayer) return;
    //std::cout << (mPlayer->Position() - mPosition).Length() << std::endl;
    mIsPlayerInRange = true ? (mPlayer->Position() - mPosition).Length() <= mDetectRange : false;
}

void Enemy::TakeDamage(int damage)
{
    if (mHealth - damage > 0) {
        mHealth -= damage;
    } else {
        mHealth = 0;
        mRemovable = true;
    }
}

void Enemy::Heal(int amount)
{
    if (mHealth + amount > mMaxHealth) {
        mHealth = mMaxHealth;
    } else {
        mHealth += amount;
    }
}

void Enemy::Update(float deltaTime)
{
    switch(mState) {
    case EnemyState::STATE_IDLE:
        if (mIsPlayerInRange) {
            mState = EnemyState::STATE_MOVE;
        }
        break;
    case EnemyState::STATE_MOVE:
        // move towards player
        Vector2 direction = mPlayer->Position() - mPosition;
        direction.Normalize();

        // apply speed and friction
        mAcceleration = direction * mSpeed;
        mAcceleration -= mVelocity * mFriction;

        // calculate new position and velocity from acceleration
        mPosition += mVelocity * deltaTime + mAcceleration * 0.5f * deltaTime * deltaTime;
        mVelocity += mAcceleration * deltaTime;
        break;
    }
}

void Enemy::DrawHealthbar(Renderer& renderer)
{

}

int Enemy::MoveTo(lua_State* L)
{
    Enemy* enemy = static_cast<Enemy*>(lua_touserdata(L, 1));

    int endPositionX = lua_tonumber(L, 2);
    int endPositionY = lua_tonumber(L, 3);
    float timeToComplete = lua_tonumber(L, 4);

    std::cout << "Enemy move\n";
    //enemy->mBehaviour.reset(new MoveToEnemyBehaviour(enemy, Vector2(endPositionX, endPositionY), timeToComplete)); // replace the old behaviour with a new behaviour

    return 0;
}