#include "ConcreteEnemyStates.h"

#include "Enemy.h"
#include "Vector2.h"
#include "Random.h"

FollowPlayer::FollowPlayer(Player* player)
{
    mPlayer = player;
}

void FollowPlayer::Update(Enemy& enemy, float deltaTime)
{
    Vector2 direction = enemy.Position() - mPlayer->Position();
    direction.Normalize();
    enemy.SetDirection(direction);
    enemy.Move(deltaTime);
}

DoNothing::DoNothing()
{
}

void DoNothing::Update(Enemy& enemy, float deltaTime)
{
    return;
}

WalkRandomly::WalkRandomly(float changeDirectionTime)
{
    mChangeDirectionTime = changeDirectionTime;
    mTimer = changeDirectionTime;
}

void WalkRandomly::Update(Enemy& enemy, float deltaTime)
{
    if (mTimer >= mChangeDirectionTime) {
        Vector2 randomDirection = Vector2(gRandom.Range(-10, 10) / 10.0f, gRandom.Range(-10, 10) / 10.0f);
        randomDirection.Normalize();
        enemy.SetDirection(randomDirection);
        mTimer = 0.0f;
    } else {
        mTimer += deltaTime;
    }

    enemy.Move(deltaTime);
}
