#ifndef CONCRETE_ENEMY_STATES_H_
#define CONCRETE_ENEMY_STATES_H_

#include "EnemyState.h"

class MoveEnemyState : public EnemyState
{
public:
    MoveEnemyState();

    void Update(Enemy& enemy, float deltaTime);

};

class IdleEnemyState : public EnemyState
{
public:
    IdleEnemyState();

    void Update(Enemy& enemy, float deltaTime);

};

class HurtEnemyState : public EnemyState
{
public:
    HurtEnemyState();

    void Update(Enemy& enemy, float deltaTime);

private:
    float mTimer;

};

#endif