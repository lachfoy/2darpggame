#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Texture.h"
#include "Vector2.h"
#include "Renderer.h"
#include "AnimationKeyFrame.h"

class Animation {
public:
    Animation(int numKeyFrames, bool startPlaying, bool looping);
    ~Animation();

    bool Playing() const { return mPlaying; }

    // methods
    void Reset() { int mCurrentFrame = 0; mPlaying = true; }
    void AddKeyFrame(AnimationKeyFrame keyFrame, int idx) { mKeyFrames[idx] = keyFrame; }
    void Update(float deltaTime);
    void DrawCurrentFrame(Renderer& renderer, Vector2 position, Texture texture);

private:
    int mNumKeyFrames;
    bool mPlaying;
    bool mLooping;
    int mCurrentFrame = 0;
    AnimationKeyFrame* mKeyFrames;
    float mCurrentTime = 0.0f;

};

#endif