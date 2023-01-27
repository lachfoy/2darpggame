#ifndef ENEMY_STATE_H_
#define ENEMY_STATE_H_

class Enemy;

class EnemyState
{
public:
    virtual ~EnemyState() {};
    virtual void Update(Enemy& enemy, float deltaTime) = 0;

};

#endif