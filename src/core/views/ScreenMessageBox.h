#ifndef _ScreenMessageBox_h_
#define _ScreenMessageBox_h_

#include <iostream>
#include "BaseScreen.h"

class GameView;
class TextRender;
class Bitmap;
class Paint;
class Canvas;



class ScreenMessageBox: public BaseScreen
{
public:
    struct OnOKClickListener
    {
        /**
        * 选择“是”后被调用
        */
        virtual void onOKClick() = 0;
    };

public:
    ScreenMessageBox(std::string msg, OnOKClickListener *l);
    virtual ~ScreenMessageBox();

    virtual void update(long delta){}

    virtual void draw(Canvas *canvas);

    virtual void onKeyDown(int key);

    virtual void onKeyUp(int key);

    virtual bool isPopup();

private:
    void _exit();

private:
    OnOKClickListener *mOnOkClickListener;
    int mIndex;

    std::string mMsg;
    Bitmap *bmpBg;
};

#endif
