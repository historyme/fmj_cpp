#ifndef _GoodsMedicineChg4Ever_H_
#define _GoodsMedicineChg4Ever_H_

#include <iostream>
class Player;

/**
 * 11仙药类
 * 永久性改变人物属性
 * @author Chen
 *
 */
class GoodsMedicineChg4Ever: public  BaseGoods
{
public:
    virtual ~GoodsMedicineChg4Ever(){}
protected :
    virtual void setOtherData(char *buf, int offset)
    {
        mMpMax = get1ByteSInt(buf, offset + 0x16);
        mHpMax = get1ByteSInt(buf, offset + 0x17);
        mdf = get1ByteSInt(buf, offset + 0x18);
        mat = get1ByteSInt(buf, offset + 0x19);
        mling = get1ByteSInt(buf, offset + 0x1a);
        mSpeed = get1ByteSInt(buf, offset + 0x1b);
        mLuck = get1ByteSInt(buf, offset + 0x1d);
    }

private:
    int mMpMax;
    int mHpMax;
    int mdf;
    int mat;
    int mling;
    int mSpeed;
    int mLuck;

public:
    virtual void eat(Player *player)
    {
        player->setMaxMP(player->getMaxMP() + mMpMax);
        player->setMaxHP(player->getMaxHP() + mHpMax);
        player->setDefend(player->getDefend() + mdf);
        player->setAttack(player->getAttack() + mat);
        player->setLingli(player->getLingli() + mling);
        player->setSpeed(player->getSpeed() + mSpeed);
        player->setLuck(player->getLuck() + mLuck);
        Player::sGoodsList->deleteGoods(mType, mIndex);
    }
};

#endif
