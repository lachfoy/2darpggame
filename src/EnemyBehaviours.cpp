#include "EnemyBehaviours.h"

#include "Enemy.h"

MoveToEnemyBehaviour::MoveToEnemyBehaviour(Enemy* enemy, Vector2 endPosition, float timeToComplete)
{
    mEnemy = enemy;
    mStartPosition = mEnemy->Position();
    mEndPosition = endPosition;
    mTimeToComplete = timeToComplete;
}

bool MoveToEnemyBehaviour::Update(float deltaTime)
{
    mTimePassed += deltaTime;

    // lerp position
    // a + (b - a) * t;
    Vector2 newPosition;
    newPosition.x = mStartPosition.x + (mEndPosition.x - mStartPosition.x) * (mTimePassed / mTimeToComplete);
    newPosition.y = mStartPosition.y + (mEndPosition.y - mStartPosition.y) * (mTimePassed / mTimeToComplete);
    mEnemy->SetPosition(newPosition);

    if (mTimePassed >= mTimeToComplete) {
        mEnemy->SetPosition(mEndPosition);
        return true;
    }

    return false;
}

WaitEnemyBehaviour::WaitEnemyBehaviour(float timeToComplete)
{
    mTimeToComplete = timeToComplete;
}

bool WaitEnemyBehaviour::Update(float deltaTime)
{
    mTimePassed += deltaTime;

    if (mTimePassed >= mTimeToComplete) {
        return true;
    }

    return false;
}

ShootEnemyBehaviour::ShootEnemyBehaviour(Enemy* enemy, int dirX, int dirY, float delay)
{
    mEnemy = enemy;
    mDirX = dirX;
    mDirY = dirY;
    mTimeToComplete = delay;
}

bool ShootEnemyBehaviour::Update(float deltaTime)
{
    mTimePassed += deltaTime;

    // if (mTimePassed >= mTimeToComplete) {
    //     ProjectileManager::Instance().CreateEnemyProjectile(
    //         mEnemy->PosX(),
    //         mEnemy->PosY(),
    //         12,
    //         12,
    //         TextureManager::Instance().GetTexture("images/fireball.png"),
    //         static_cast<float>(mDirX),
    //         static_cast<float>(mDirY),
    //         10.f,
    //         10
    //     );
    //     return true;
    // }

    return false;
}