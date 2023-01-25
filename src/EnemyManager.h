#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include <glm/glm.hpp>
#include <list>
#include "Renderer.h"
#include "Enemy.h"
#include <memory>
#include "Map.h"
#include "Player.h"

typedef std::unique_ptr<Enemy> EnemyPtr;

class EnemyManager
{
public:
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    ~EnemyManager();

    const std::list<EnemyPtr>& Enemies() const { return mEnemies; }

    void CreateEnemy(int x, int y, const char* enemyScript);
    void UpdateEnemies(Map& map, Player& player);
    void HousekeepEnemies();
    void DrawEnemies(Renderer& renderer);
    void DrawEnemyHealthbars(Renderer& renderer);

private:
    EnemyManager() = default; // IMPORTANT FOR INITALIZING THE LIST
    
    std::list<EnemyPtr> mEnemies;

};

#endif