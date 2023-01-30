#ifndef OBSERVER_H_
#define OBSERVER_H_

class Player;

class Observer
{
public:
    virtual ~Observer() {}
    virtual void OnNotify(const Player& player, int event) = 0;

};

#endif