#ifndef _WalkingSprite_H_
#define _WalkingSprite_H_

#include <iostream>
#include "Global.h"
#include "DatLib.h"
#include "ResImage.h"
#include "Canvas.h"
#include "Direction.h"

class WalkingSprite
{
public:
    WalkingSprite(int type, int id);
    virtual ~WalkingSprite();
    int getId();

    void setDirection(Direction d);

    void walk(Direction d);

    void walk();

    void setStep(int step);

    int getStep();

    void draw(Canvas *canvas, int x, int y);

private:
    static const int OFFSET[4];

    ResImage *mResImage;

    int mOffset; // 面向
    int mI; // 脚步
};
#endif
