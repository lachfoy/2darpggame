#include "Animation.h"

Animation::Animation(int numKeyFrames, bool startPlaying, bool looping) : mNumKeyFrames(numKeyFrames), mPlaying(startPlaying), mLooping(looping)
{
    mKeyFrames = new AnimationKeyFrame[numKeyFrames];
}

Animation::~Animation()
{
    delete[] mKeyFrames;
}

void Animation::Update(float deltaTime) {
    if (!mPlaying) return;
    if (mCurrentTime < 0.08333333333f) {
        mCurrentTime += deltaTime;
    } else {
        mCurrentTime = 0.0f;
        if (mCurrentFrame == mNumKeyFrames - 1) {
            if (mLooping)
                mCurrentFrame = 0;
            else
                mPlaying = false;
        } else {
            mCurrentFrame++;
        }
    }
}

void Animation::DrawCurrentFrame(Renderer& renderer, Vector2 position, Texture texture)
{
    renderer.DrawPartialSprite(
        position.x,
        position.y,
        mKeyFrames[mCurrentFrame].x,
        mKeyFrames[mCurrentFrame].y,
        mKeyFrames[mCurrentFrame].w,
        mKeyFrames[mCurrentFrame].h,
        texture
    );
}