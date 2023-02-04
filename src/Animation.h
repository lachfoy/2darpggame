#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Texture.h"
#include "Vector2.h"
#include "Renderer.h"
#include "AnimationKeyFrame.h"

class Animation {
public:
    Animation(int numKeyFrames);
    ~Animation();

    // methods
    void AddKeyFrame(AnimationKeyFrame keyFrame, int idx) { mKeyFrames[idx] = keyFrame; }
    void Update(float deltaTime);
    void DrawCurrentFrame(Renderer& renderer, Vector2 position, Texture texture);

private:
    bool mPlaying = true;
    int mCurrentFrame = 0;
    int mNumKeyFrames;
    AnimationKeyFrame* mKeyFrames;
    float mCurrentTime = 0.0f;

};

#endif