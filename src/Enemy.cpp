#include "Enemy.h"

#include <cmath> // fabs
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

Enemy::Enemy(int x, int y, const char* enemyScript) : Actor(x, y, 0)
{
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

    mGreenHealthbar = false;

    mDamage = lua_tonumber(L, -1);

    lua_close(L);
}

Enemy::~Enemy()
{

}

void Enemy::OnDeath()
{
    mRemovable = true;
}

void Enemy::Update(Map& map, Player& player, std::list<EnemyPtr>& otherEnemies)
{
    // if not on top of player
    // then pick a random direction to move
    
    switch(gRandom.Range(0, 3)) {
    case 0:
        if (mPosY - 1 != player.PosY()) MoveUp(map);
        break;
    case 1:
        if (mPosY + 1 != player.PosY()) MoveDown(map);
        break;
    case 2:
        if (mPosX - 1 != player.PosX()) MoveLeft(map);
        break;
    case 3:
        if (mPosX + 1 != player.PosX()) MoveRight(map);
        break;
    }

    // next to player
    if ((mPosX + 1 == player.PosX() && mPosY == player.PosY()) ||
        (mPosX - 1 == player.PosX() && mPosY == player.PosY()) ||
        (mPosX == player.PosX() && mPosY + 1 == player.PosY()) ||
        (mPosX == player.PosX() && mPosY - 1 == player.PosY())) {
        std::cout << "attack\n";

        player.TakeDamage(mDamage);
        TakeDamage(player.GetDamage());
    }

    // otherwise if on top of the player, exchange attacks. 
    // i.e., deal damage to the player according to damage stat
    // take damage from the player according to player's damage stat
    // force the enemy to wait a turn after attacking?
}