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
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    if (luaL_dofile(L, enemyScript) != 0) {
        // abort, idk. todo: error checking, put the program back into a safe state
        std::cout << lua_tostring(L, -1) << std::endl; // error at top of stack
    }

    lua_getglobal(L, "texture");
    lua_getglobal(L, "max_health");
    lua_getglobal(L, "damage");

    // get texture
    mTexture = TextureManager::Instance().GetTexture(lua_tostring(L, -3));

    mId = GetId();

    // get max health value
    mMaxHealth = lua_tonumber(L, -2);
    mHealth = mMaxHealth;

    mDamage = lua_tonumber(L, -1);

    lua_close(L);
}

Enemy::~Enemy()
{

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

void Enemy::Update(Player& player)
{

}

void Enemy::DrawHealthbar(Renderer& renderer)
{

}