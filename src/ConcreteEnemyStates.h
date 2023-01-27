#ifndef CONCRETE_ENEMY_STATES_H_
#define CONCRETE_ENEMY_STATES_H_

#include "EnemyState.h"

#include "Player.h"

class FollowPlayer : public EnemyState
{
public:
    FollowPlayer(Player* player);

    void Update(Enemy& enemy, float deltaTime) override;

private:
    Player* mPlayer;

};

class DoNothing : public EnemyState
{
public:
    DoNothing();

    void Update(Enemy& enemy, float deltaTime) override;

};

class WalkRandomly : public EnemyState
{
public:
    WalkRandomly(float changeDirectionTime);

    void Update(Enemy& enemy, float deltaTime) override;

private:
    float mChangeDirectionTime;
    float mTimer;

};

#endif