#include "Animation.h"

Animation::Animation(int numKeyFrames) : mNumKeyFrames(numKeyFrames)
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
        mCurrentFrame = mCurrentFrame == mNumKeyFrames - 1 ? 0 : mCurrentFrame + 1;
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