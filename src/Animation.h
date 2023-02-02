#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Texture.h"
#include "Vector2.h"
#include <list>

struct KeyFrame
{
    float texCoords[8];
    int duration;
};

class Animation {
public:
    Animation() {}
    ~Animation() {}

    void Update(float deltaTime) {
        if (currentTime >= frameTime) {
            currentTime = 0.0f;
            currentFrame++;
            if (currentFrame == keyFrames[i].duration) {
                currentFrame = 0;
                i++; // next keyframe
            }
        } else {
            currentTime += deltaTime;
        }
    }

    void DrawCurrentFrame()
    {

    }

    int i = 0;
    int currentFrame = 0;
    Texture texture;
    const int numKeyFrames = 3;
    KeyFrame* keyFrames;
    const int fps = 30;
    float frameTime = fps / 1000.0f;
    float currentTime = 0.0f;

};

#endif