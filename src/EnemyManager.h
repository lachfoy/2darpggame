#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include <list>
#include "Vector2.h"
#include <glad/glad.h>
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

    // getters
    const std::list<EnemyPtr>& Enemies() const { return mEnemies; }

    // methods
    void CreateEnemy(Vector2 position, Player* player, const char* enemyScript);
    void UpdateEnemies(float deltaTime);
    void HousekeepEnemies();
    void DrawEnemies(Renderer& renderer);
    void DrawEnemyHealthbars(Renderer& renderer);

private:
    EnemyManager() = default; // IMPORTANT FOR INITALIZING THE LIST
    
    std::list<EnemyPtr> mEnemies;

};

#endif