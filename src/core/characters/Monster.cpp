#include "Monster.h"


Monster::Monster()
{
    bInstanceof_Monster = true;
}

Monster::~Monster()
{

}

void Monster::setData(char *buf, int offset)
{
    mType = (int)buf[offset] & 0xff;
    mIndex = (int)buf[offset + 1] & 0xff;
    ResMagicChain *magicChain = (ResMagicChain *)DatLib::GetRes(DatLib::RES_MLR, 1, (int)buf[offset + 0x2f] & 0xff);
    if (magicChain != NULL)
    {
        magicChain->setLearnNum((int)buf[offset + 2] & 0xff);
        setMagicChain(magicChain);
    }
    addBuff((int)buf[offset + 3] & 0xff);
    mAtbuff = (int)buf[offset + 4] & 0xff;
    mLastRound = (int)buf[offset + 0x17] & 0xff;
    setName(getString(buf, offset + 6));
    setLevel((int)buf[offset + 0x12] & 0xff);
    setMaxHP(get2BytesInt(buf, offset + 0x18));
    setHP(get2BytesInt(buf, offset + 0x1a));
    setMaxMP(get2BytesInt(buf, offset + 0x1c));
    setMP(get2BytesInt(buf, offset + 0x1e));
    setAttack(get2BytesInt(buf, offset + 0x20));
    setDefend(get2BytesInt(buf, offset + 0x22));
    setSpeed((int)buf[offset + 0x13] & 0xff);
    setLingli((int)buf[offset + 0x14] & 0xff);
    setLuck((int)buf[offset + 0x16] & 0xff);
    mIQ = (int)buf[offset + 0x15] & 0xff;
    mMoney = get2BytesInt(buf, offset + 0x24);
    mEXP = get2BytesInt(buf, offset + 0x26);
    mCarryGoods1[0] = (int)buf[offset + 0x28] & 0xff;
    mCarryGoods1[1] = (int)buf[offset + 0x29] & 0xff;
    mCarryGoods1[2] = (int)buf[offset + 0x2a] & 0xff;
    mCarryGoods2[0] = (int)buf[offset + 0x2b] & 0xff;
    mCarryGoods2[1] = (int)buf[offset + 0x2c] & 0xff;
    mCarryGoods2[2] = (int)buf[offset + 0x2d] & 0xff;
    setFightingSprite(new FightingSprite(DatLib::RES_ACP, (int)buf[offset + 0x2e] & 0xff));
}

BaseGoods * Monster::getDropGoods()
{
    if (mCarryGoods2[0] == 0 || mCarryGoods2[1] == 0 || mCarryGoods2[2] == 0)
    {
        return NULL;
    }

    BaseGoods *g = (BaseGoods *)DatLib::GetRes(DatLib::RES_GRS, mCarryGoods2[0], mCarryGoods2[1]);
    g->setGoodsNum(mCarryGoods2[2]);
    return g;
}

void Monster::setOriginalCombatPos(int i)
{
    FightingSprite *fs = getFightingSprite();
    fs->setCombatPos(MONSTRTPOS[i][0] - (fs->getWidth() / 6) + fs->getWidth() / 2,
        MONSTRTPOS[i][1] - (fs->getHeight() / 10) + fs->getHeight() / 2);
}

const int Monster::MONSTRTPOS[3][2] = { { 12, 25 }, { 44, 14 }, { 82, 11 } };
