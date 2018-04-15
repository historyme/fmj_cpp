#pragma once
#include <iostream>
#include "Bitmap.h"
#include "BaseScreen.h"
#include "HMStack.h"
#include "Point.h"

class Player;
class GoodsEquipment;

using namespace std;

class  ScreenChgEquipment : public BaseScreen
{
private:
    Player *mActor;
    vector<GoodsEquipment *> mGoods;
    int mSelIndex;
    int mPage;

    /**
     *
     * @param actorList 可装备选择的物品的角色链表
     * @param goods 选择的物品
     */
public:
    ScreenChgEquipment(Player *actor, GoodsEquipment *goods);
    ~ScreenChgEquipment(){}

    virtual void update(long delta){}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key);
    virtual void onKeyUp(int key);
};
