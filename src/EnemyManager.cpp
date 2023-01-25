#include "EnemyManager.h"

#include <iostream>

EnemyManager::~EnemyManager()
{
    std::cout << "EnemyManager:: Destructor called!\n";
    mEnemies.clear();
}

void EnemyManager::CreateEnemy(int x, int y, const char* enemyScript)
{
    std::cout << "EnemyManager:: Creating new enemy\n";
    mEnemies.push_back(EnemyPtr(new Enemy(x, y, enemyScript)));
}

void EnemyManager::UpdateEnemies(Map& map, Player& player)
{
    player.Hi();

    for (const auto& enemy : mEnemies) {
        enemy->Update(map, player, mEnemies);
    }
}

void EnemyManager::HousekeepEnemies()
{
    mEnemies.remove_if([&](const EnemyPtr& enemy) -> bool { return enemy->Removable(); });
}

void EnemyManager::DrawEnemies(Renderer& renderer)
{
    for (const auto& enemy : mEnemies) {
        enemy->Draw(renderer);
    }
}

void EnemyManager::DrawEnemyHealthbars(Renderer& renderer)
{
    for (const auto& enemy : mEnemies) {
        enemy->DrawHealthbar(renderer);
    }
}