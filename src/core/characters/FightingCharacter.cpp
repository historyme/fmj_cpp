#include "FightingCharacter.h"


FightingCharacter::FightingCharacter()
{
    bInstanceof_Monster = false;
    bInstanceof_Player = false;
    mIsVisiable = true;

    memset(mBuff, 0x00, sizeof(mBuff));
    memset(mBuffRound, 0x00, sizeof(mBuffRound));
    memset(mDebuffRound, 0x00, sizeof(mDebuffRound));
    memset(mAtbuffRound, 0x00, sizeof(mAtbuffRound));
    mDebuff = 0x00;
    mAtbuff = 0x00;
}

FightingCharacter::~FightingCharacter()
{
    if (NULL != mFightingSprite)
    {
        delete mFightingSprite;
    }
    
    if (NULL != mMagicChain)
    {
        delete mMagicChain;
    }
}

FightingSprite * FightingCharacter::getFightingSprite()
{
    return mFightingSprite;
}

void FightingCharacter::setFightingSprite(FightingSprite *fightingSprite)
{
    mFightingSprite = fightingSprite;
}

void FightingCharacter::setCombatPos(int x, int y)
{
    mFightingSprite->setCombatPos(x, y);
}

int FightingCharacter::getCombatX()
{
    return mFightingSprite->getCombatX();
}

int FightingCharacter::getCombatY()
{
    return mFightingSprite->getCombatY();
}

int FightingCharacter::getCombatLeft()
{
    return mFightingSprite->getCombatX() - mFightingSprite->getWidth() / 2;
}

int FightingCharacter::getCombatTop()
{
    return mFightingSprite->getCombatY() - mFightingSprite->getHeight() / 2;
}

void FightingCharacter::setMagicChain(ResMagicChain *magicChain)
{
    mMagicChain = magicChain;
}

ResMagicChain * FightingCharacter::getMagicChain()
{
    return mMagicChain;
}

void FightingCharacter::setLevel(int level)
{
    mLevel = level;
}

int FightingCharacter::getLevel()
{
    return mLevel;
}

void FightingCharacter::setMaxHP(int maxHP)
{
    if (maxHP > 999)
    {
        maxHP = 999;
    }
    mMaxHP = maxHP;
}

int FightingCharacter::getMaxHP()
{
    return mMaxHP;
}

void FightingCharacter::setMaxMP(int maxMP)
{
    if (maxMP > 999)
    {
        maxMP = 999;
    }
    mMaxMP = maxMP;
}

int FightingCharacter::getMaxMP()
{
    return mMaxMP;
}

void FightingCharacter::setHP(int hp)
{
    if (hp > mMaxHP)
    {
        hp = mMaxHP;
    }
    mHP = hp;
}

int FightingCharacter::getHP()
{
    return mHP;
}

bool FightingCharacter::isAlive()
{
    return mHP > 0;
}

bool FightingCharacter::isVisiable()
{
    return mIsVisiable;
}

void FightingCharacter::setMP(int mp)
{
    if (mp > mMaxMP)
    {
        mp = mMaxMP;
    }
    mMP = mp;
}

void FightingCharacter::setAttack(int at)
{
    if (at > 999)
    {
        at = 999;
    }
    mAttack = at;
}

void FightingCharacter::setDefend(int d)
{
    if (d > 999)
    {
        d = 999;
    }
    mDefend = d;
}

void FightingCharacter::setVisiable(bool visiable)
{
    mIsVisiable = visiable;
}

int FightingCharacter::getMP()
{
    return mMP;
}

int FightingCharacter::getAttack()
{
    return mAttack;
}

int FightingCharacter::getDefend()
{
    return mDefend;
}

void FightingCharacter::setSpeed(int s)
{
    if (s > 99)
    {
        s = 99;
    }
    mSpeed = s;
}

int FightingCharacter::getSpeed()
{
    return mSpeed;
}

int FightingCharacter::getLingli()
{
    return mlingli;
}

void FightingCharacter::setLingli(int l)
{
    if (l > 99)
    {
        l = 99;
    }
    mlingli = l;
}

void FightingCharacter::setLuck(int l)
{
    if (l > 99)
    {
        l = 99;
    }
    mLuck = l;
}

int FightingCharacter::getLuck()
{
    return mLuck;
}

bool FightingCharacter::hasBuff(int mask)
{
    if ((mask & BUFF_MASK_DU) == BUFF_MASK_DU && mBuff[0] <= 0)
    {
        return false;
    }
    if ((mask & BUFF_MASK_LUAN) == BUFF_MASK_LUAN && mBuff[1] <= 0)
    {
        return false;
    }
    if ((mask & BUFF_MASK_FENG) == BUFF_MASK_FENG && mBuff[2] <= 0)
    {
        return false;
    }
    if ((mask & BUFF_MASK_MIAN) == BUFF_MASK_MIAN && mBuff[3] <= 0)
    {
        return false;
    }
    if ((mask & BUFF_MASK_ALL) == BUFF_MASK_ALL && mBuff[4] <= 0)
    {
        return false;
    }
    return true;
}

bool FightingCharacter::hasDebuff(int mask)
{
    return (mDebuff & mask) != 0;
}

bool FightingCharacter::hasAtbuff(int mask)
{
    return (mAtbuff & mask) == mask;
}

void FightingCharacter::addBuff(int mask)
{
    addBuff(mask, INT_MAX);
}

void FightingCharacter::addBuff(int mask, int rounds)
{
    if ((mask & BUFF_MASK_DU) == BUFF_MASK_DU)
    {
        ++mBuff[0];
        mBuffRound[0] = rounds;
    }
    if ((mask & BUFF_MASK_LUAN) == BUFF_MASK_LUAN)
    {
        ++mBuff[1];
        mBuffRound[1] = rounds;
    }
    if ((mask & BUFF_MASK_FENG) == BUFF_MASK_FENG)
    {
        ++mBuff[2];
        mBuffRound[2] = rounds;
    }
    if ((mask & BUFF_MASK_MIAN) == BUFF_MASK_MIAN)
    {
        ++mBuff[3];
        mBuffRound[3] = rounds;
    }
}

void FightingCharacter::delBuff(int mask)
{
    if ((mask & BUFF_MASK_DU) == BUFF_MASK_DU)
    {
        if (--mBuff[0] < 0)
        {
            mBuff[0] = 0;
        }
    }
    if ((mask & BUFF_MASK_LUAN) == BUFF_MASK_LUAN)
    {
        if (--mBuff[1] < 0)
        {
            mBuff[1] = 0;
        }
    }
    if ((mask & BUFF_MASK_FENG) == BUFF_MASK_FENG)
    {
        if (--mBuff[2] < 0)
        {
            mBuff[2] = 0;
        }
    }
    if ((mask & BUFF_MASK_MIAN) == BUFF_MASK_MIAN)
    {
        if (--mBuff[3] < 0)
        {
            mBuff[3] = 0;
        }
    }
}

int FightingCharacter::getBuffRound(int mask)
{
    if ((mask & BUFF_MASK_DU) == BUFF_MASK_DU)
    {
        return mBuffRound[0];
    }
    if ((mask & BUFF_MASK_LUAN) == BUFF_MASK_LUAN)
    {
        return mBuffRound[1];
    }
    if ((mask & BUFF_MASK_FENG) == BUFF_MASK_FENG)
    {
        return mBuffRound[2];
    }
    if ((mask & BUFF_MASK_MIAN) == BUFF_MASK_MIAN)
    {
        return mBuffRound[3];
    }
    return 0;
}

void FightingCharacter::addDebuff(int mask, int rounds)
{
    mDebuff |= mask;
    if ((mask & BUFF_MASK_DU) == BUFF_MASK_DU)
    {
        mDebuffRound[0] = rounds;
    }
    if ((mask & BUFF_MASK_LUAN) == BUFF_MASK_LUAN)
    {
        mDebuffRound[1] = rounds;
    }
    if ((mask & BUFF_MASK_FENG) == BUFF_MASK_FENG)
    {
        mDebuffRound[2] = rounds;
    }
    if ((mask & BUFF_MASK_MIAN) == BUFF_MASK_MIAN)
    {
        mDebuffRound[3] = rounds;
    }
}

void FightingCharacter::addAtbuff(int mask, int rounds)
{
    mAtbuff |= mask;
    if ((mask & BUFF_MASK_DU) == BUFF_MASK_DU)
    {
        mAtbuffRound[0] = rounds;
    }
    if ((mask & BUFF_MASK_LUAN) == BUFF_MASK_LUAN)
    {
        mAtbuffRound[1] = rounds;
    }
    if ((mask & BUFF_MASK_FENG) == BUFF_MASK_FENG)
    {
        mAtbuffRound[2] = rounds;
    }
    if ((mask & BUFF_MASK_MIAN) == BUFF_MASK_MIAN)
    {
        mAtbuffRound[3] = rounds;
    }
}

void FightingCharacter::delAtbuff(int mask)
{
    mAtbuff &= (~mask);
}
