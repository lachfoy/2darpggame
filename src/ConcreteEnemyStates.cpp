#include "ConcreteEnemyStates.h"

#include "Enemy.h"
#include "Vector2.h"

MoveEnemyState::MoveEnemyState(){}

void MoveEnemyState::Update(Enemy& enemy, float deltaTime)
{
    if (enemy.mDirection != Vector2::Zero()) {
        enemy.mDirection.Normalize();
        enemy.mPosition.x += enemy.mDirection.x * enemy.mSpeed * deltaTime;
        enemy.mPosition.y += enemy.mDirection.y * enemy.mSpeed * deltaTime;
    }
}

IdleEnemyState::IdleEnemyState(){}

void IdleEnemyState::Update(Enemy& enemy, float deltaTime)
{
    return;
}
