#ifndef _Player_H_
#define _Player_H_

#include <iostream>
#include "FightingCharacter.h"
#include "GoodsManage.h"

class ResLevelupChain;
class GoodsEquipment;
class Canvas;
class SaveLoadStream;

class Player : public FightingCharacter
{
public:
    Player();
    virtual ~Player();
    void drawHead(Canvas *canvas, int x, int y);

    void setFrameByState();

    virtual void setData(char *buf, int offset);

    ResLevelupChain *getLevelupChain();


    void setCurrentExp(int exp);

    int getCurrentExp()
    {
        return mCurrentExp;
    }


    GoodsEquipment *getCurrentEquipment(int type);

    GoodsEquipment **getEquipmentsArray();

    /**
     * 是否已经装备该装备，对装饰检测空位
     * @param type
     * @param id
     * @return
     */
    bool hasEquipt(int type, int id);

    /**
     * 穿上goods装备
     * @param goods
     */
    void putOn(GoodsEquipment *goods);

    /**
     * 脱下类型号为type的装备
     * @param type
     */
    void takeOff(int type);

    /**
     * type型装备位置是否已经有装备
     * @param type 装备类型号 {@link GoodsEquipment#getType()}
     * @return 是否有空
     */
    bool hasSpace(int type);

    void drawState(Canvas *canvas, int page);

private:

    ResImage *mImgHead;
    ResLevelupChain *mLevelupChain;
    int mCurrentExp; // 当前经验值

    /**
    * 0装饰 1装饰 2护腕 3脚蹬 4手持 5身穿 6肩披 7头戴
    */
    GoodsEquipment *mEquipments[8];

public:
    /** 装备界面从左至右的装备类型号*/
    static const int sEquipTypes[8];

    static GoodsManage *sGoodsList;
    static int sMoney;

public:
    void read(SaveLoadStream *buff);
    void write(SaveLoadStream *buff);
};

#endif
