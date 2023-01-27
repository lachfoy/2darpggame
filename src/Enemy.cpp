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

Enemy::Enemy(Vector2 position, const char* enemyScript)
{
    mPosition = position;

    // open Lua file
    mLuaState = luaL_newstate();
    luaL_openlibs(mLuaState);

    lua_register(mLuaState, "MoveTo", MoveTo); // expose api function to lua

    // run the lua file
    if (luaL_dofile(mLuaState, enemyScript) != 0) {
        std::cout << lua_tostring(mLuaState, -1) << "\n"; // error at top of stack
    }

    // query values 
    lua_getglobal(mLuaState, "texture");
    lua_getglobal(mLuaState, "speed");
    lua_getglobal(mLuaState, "damage");
    lua_getglobal(mLuaState, "max_health");

    // get values from the stack
    mTexture = TextureManager::Instance().GetTexture(lua_tostring(mLuaState, -4));
    mSpeed = lua_tonumber(mLuaState, -3);
    mDamage = lua_tonumber(mLuaState, -2);
    mMaxHealth = lua_tonumber(mLuaState, -1);
    mHealth = mMaxHealth;

    mId = GetId();
}

Enemy::~Enemy()
{
    lua_close(mLuaState);
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
    if (!mEnemyState) return;

    mEnemyState->Update(deltaTime);

    if (mDirection != Vector2::Zero()) {
        mDirection.Normalize();
        mPosition.x += mDirection.x * mSpeed * deltaTime;
        mPosition.y += mDirection.y * mSpeed * deltaTime;
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
    enemy->mBehaviour.reset(new MoveToEnemyBehaviour(enemy, Vector2(endPositionX, endPositionY), timeToComplete)); // replace the old behaviour with a new behaviour

    return 0;
}