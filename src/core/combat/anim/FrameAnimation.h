#ifndef _FrameAnimation_h_
#define _FrameAnimation_h_

#include <iostream>
#include "ResImage.h"
#include "Canvas.h"


/** 帧动画 */
class FrameAnimation
{
private:
    int DELTA;
	ResImage *mImage;
	int mStartFrame;
	int mEndFrame;
	int mCurFrame;
	long mTimeCnt;
	
public:
    FrameAnimation(ResImage *img);	
	FrameAnimation(ResImage *img, int startFrame, int endFrame);
    virtual ~FrameAnimation(){}
	
	void setFPS(int fps);
	void update(long delta);
	void draw(Canvas *canvas, int x, int y);
};

#endif
