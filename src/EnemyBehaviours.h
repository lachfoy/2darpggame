#ifndef ENEMY_BEHAVIOURS_H_
#define ENEMY_BEHAVIOURS_H_

//#include "ProjectileManager.h"
#include "Vector2.h"

class Enemy;

class EnemyBehaviour
{
public:
    virtual ~EnemyBehaviour() {};

    virtual bool Update(float deltaTime) = 0;

protected:
    Enemy* mEnemy;

};

class MoveToEnemyBehaviour : public EnemyBehaviour
{
public:
    MoveToEnemyBehaviour(Enemy* enemy, Vector2 endPosition, float timeToComplete);

    bool Update(float deltaTime) override;

private:
    Vector2 mStartPosition;
    Vector2 mEndPosition;
    float mTimePassed = 0.f;
    float mTimeToComplete;

};

class WaitEnemyBehaviour : public EnemyBehaviour
{
public:
    WaitEnemyBehaviour(float timeToComplete);

    bool Update(float deltaTime) override;

private:
    float mTimePassed = 0.f;
    float mTimeToComplete;

};

class ShootEnemyBehaviour : public EnemyBehaviour
{
public:
    ShootEnemyBehaviour(Enemy* enemy, int dirX, int dirY, float delay);

    bool Update(float deltaTime) override;

private:
    int mDirX;
    int mDirY;
    float mTimePassed = 0.f;
    float mTimeToComplete;

};

#endif