#include "GoodsEquipment.h"

#include "Player.h"


GoodsEquipment::GoodsEquipment()
{
    bFatherOfGoodsWeapon = false;
}

void GoodsEquipment::setOtherData(char * buf, int offset)
{
    mMpMax = get1ByteSInt(buf, offset + 0x16);
    mHpMax = get1ByteSInt(buf, offset + 0x17);
    mdf = get1ByteSInt(buf, offset + 0x18);
    mat = (int)buf[offset + 0x19] & 0xff;
    mlingli = get1ByteSInt(buf, offset + 0x1a);
    mSpeed = get1ByteSInt(buf, offset + 0x1b);
    mBitEffect = (int)buf[offset + 0x1c] & 0xff;
    mLuck = get1ByteSInt(buf, offset + 0x1d);
}

void GoodsEquipment::putOn(Player *p)
{
    if (canPlayerUse(p->getIndex()))
    {
        p->setMaxMP(p->getMaxMP() + mMpMax);
        p->setMaxHP(p->getMaxHP() + mHpMax);
        p->setDefend(p->getDefend() + mdf);
        p->setAttack(p->getAttack() + mat);
        p->setLingli(p->getLingli() + mlingli);
        p->setSpeed(p->getSpeed() + mSpeed);

        //判断当前是不是GoodsWeapon类的拷贝
        //这里可以用一个标记来做，只有GoodsWeapon会标记为1，这样就能够识别了
        //if (!(this instanceof GoodsWeapon))
        if (!bFatherOfGoodsWeapon)
        {
            p->addBuff(mBitEffect); // 添加免疫效果
        }
        p->setLuck(p->getLuck() + mLuck);
        if (getEventId() != 0)
        {
            // 设置装备触发的事件
            ScriptResources::setEvent(getEventId());
        }
    }
}

void GoodsEquipment::takeOff(Player *p)
{
    p->setMaxMP(p->getMaxMP() - mMpMax);
    p->setMaxHP(p->getMaxHP() - mHpMax);
    p->setDefend(p->getDefend() - mdf);
    p->setAttack(p->getAttack() - mat);
    p->setLingli(p->getLingli() - mlingli);
    p->setSpeed(p->getSpeed() - mSpeed);

    //判断当前是不是GoodsWeapon类的拷贝
    if (!bFatherOfGoodsWeapon)
    {
        p->delBuff(mBitEffect); // 删掉免疫效果
    }
    p->setLuck(p->getLuck() - mLuck);
    if (getEventId() != 0)
    {
        // 取消该事件
        ScriptResources::clearEvent(getEventId());
    }
}
