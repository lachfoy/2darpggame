#include "ConcreteEnemyStates.h"

#include "Enemy.h"
#include "Vector2.h"
#include "Random.h"

FollowPlayer::FollowPlayer(Enemy* enemy, Player* player) : EnemyState(enemy)
{
    mPlayer = player;
}

void FollowPlayer::Update(float deltaTime)
{
    Vector2 direction = mEnemy->Position() - mPlayer->Position();
    direction.Normalize();
    mEnemy->SetDirection(direction);
}

DoNothing::DoNothing(Enemy* enemy) : EnemyState(enemy)
{
    mEnemy->SetDirection(Vector2::Zero());
}

void DoNothing::Update(float deltaTime)
{
    return;
}

WalkRandomly::WalkRandomly(Enemy* enemy, float changeDirectionTime) : EnemyState(enemy)
{
    mChangeDirectionTime = changeDirectionTime;
    mTimer = 0.0f;
}

void WalkRandomly::Update(float deltaTime)
{
    if (mTimer >= mChangeDirectionTime) {
        Vector2 randomDirection = Vector2(gRandom.Range(-10, 10) / 10.0f, gRandom.Range(-10, 10) / 10.0f);
        randomDirection.Normalize();
        mEnemy->SetDirection(randomDirection);
        mTimer = 0.0f;
    } else {
        mTimer += deltaTime;
    }
}
