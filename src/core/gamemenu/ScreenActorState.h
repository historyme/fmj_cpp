#pragma once
#include <iostream>
#include "Bitmap.h"
#include "BaseScreen.h"

class Player;

using namespace std;

class  ScreenActorState : public BaseScreen
{
private:
    int mPage;
    vector<Player *> mPlayerList;
    int mCurPlayer;

public:
    ScreenActorState();
    ~ScreenActorState();

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
};
