#include "FrameAnimation.h"


FrameAnimation::FrameAnimation(ResImage *img)
{
    DELTA = 1000 / 5;
    mTimeCnt = 0;

    mImage = img;
    mStartFrame = 1;
    mEndFrame = img->getNumber();
    mCurFrame = 1;
}

FrameAnimation::FrameAnimation(ResImage *img, int startFrame, int endFrame)
{
    DELTA = 1000 / 5;
    mTimeCnt = 0;

    mImage = img;
    mStartFrame = startFrame;
    mEndFrame = endFrame;
    mCurFrame = startFrame;
}

void FrameAnimation::setFPS(int fps)
{
    DELTA = 1000 / fps;
}

void FrameAnimation::update(long delta)
{
    mTimeCnt += delta;
    if (mTimeCnt >= DELTA) {
        mTimeCnt = 0;

        if (++mCurFrame > mEndFrame)
        {
            mCurFrame = mStartFrame;
        }
    }
}

void FrameAnimation::draw(Canvas *canvas, int x, int y)
{
    mImage->draw(canvas, mCurFrame, x, y);
}
