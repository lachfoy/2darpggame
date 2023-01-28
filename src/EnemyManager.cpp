#include "EnemyManager.h"

#include <iostream>

EnemyManager::~EnemyManager()
{
    std::cout << "EnemyManager:: Destructor called!\n";
    mEnemies.clear();
}

void EnemyManager::CreateEnemy(Vector2 position, Player* player, const char* enemyScript)
{
    std::cout << "EnemyManager:: Creating new enemy\n";
    mEnemies.push_back(EnemyPtr(new Enemy(position, player, enemyScript)));
}

void EnemyManager::UpdateEnemies(float deltaTime)
{
    for (const auto& enemy : mEnemies) {
        enemy->Update(deltaTime);
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