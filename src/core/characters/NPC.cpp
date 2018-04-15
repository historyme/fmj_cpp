#include "NPC.h"
#include "SaveLoadStream.h"

NPC::NPC()
{
    mActiveCnt = 0;
    mWalkingCnt = 0;

    bInstanceOf_SceneObj = false;
}

NPC::~NPC()
{

}

void NPC::setData(char *buf, int offset)
{
    mType = (int)buf[offset] & 0xFF;
    mIndex = (int)buf[offset + 1] & 0xFF;
    // 动作状态
    setCharacterState((int)buf[offset + 4] & 0xFF);
    // 姓名
    setName(getString(buf, offset + 9));
    // 延时
    mDelay = (int)buf[offset + 0x15] & 0xFF;
    if (mDelay == 0)
    {
        setCharacterState(STATE_STOP);
    }

    mPauseCnt = mDelay * 100;

    // 行走图
    setWalkingSprite(new WalkingSprite(2, (int)buf[offset + 0x16] & 0xFF));
    // 面向
    int faceto = (int)buf[offset + 2] & 0xFF;
    Direction d = North;
    switch (faceto)
    {
    case 1:
        d = North;
        break;
    case 2:
        d = East;
        break;
    case 3:
        d = South;
        break;
    case 4:
        d = West;
        break;
    }
    setDirection(d);
    // 脚步
    setStep((int)buf[offset + 3] & 0xFF);
}

void NPC::update(long delta)
{
    switch (getCharacterState())
    {
    case STATE_PAUSE:
        mPauseCnt -= delta;
        if (mPauseCnt < 0)
        {
            setCharacterState(STATE_WALKING);
        }
        break;

    case STATE_FORCE_MOVE:
    case STATE_WALKING:
        mWalkingCnt += delta;
        if (mWalkingCnt < 500) break;
        mWalkingCnt = 0;
        if (mRandom.nextInt(5) == 0)   // 五分之一的概率暂停
        {
            mPauseCnt = mDelay * 100;
            setCharacterState(STATE_PAUSE);
        }
        else if (mRandom.nextInt(5) == 0)     // 五分之一的概率改变方向
        {
            int i = mRandom.nextInt(4);
            Direction d = North;
            switch (i)
            {
            case 0:
                d = North;
                break;
            case 1:
                d = East;
                break;
            case 2:
                d = South;
                break;
            case 3:
                d = West;
                break;
            }
            setDirection(d);
            walk();
        }
        else
        {
            walk();
        }
        break;

    case STATE_STOP:
        break;

    case STATE_ACTIVE:
        mActiveCnt += delta;
        if (mActiveCnt > 100)
        {
            mActiveCnt = 0;
            walkStay();
        }
        break;
    }
}

void NPC::walk()
{
    int x = getPosInMap().x;
    int y = getPosInMap().y;
    switch (getDirection())
    {
    case North:
        --y;
        break;
    case East:
        ++x;
        break;
    case South:
        ++y;
        break;
    case West:
        --x;
        break;
    }
    if (mCanWalk->canWalk(x, y))
    {
        //调用父类的方法
        Character::walk();
    }
}

void NPC::read(SaveLoadStream *buff)
{
    mType = buff->readInt();
    mIndex = buff->readInt();
    setCharacterState(buff->readInt());
    setName(buff->readString());
    mDelay = buff->readInt();
    setWalkingSprite(new WalkingSprite(2, buff->readInt()));

    int d = buff->readInt();
    setDirection((Direction)d);

    setStep(buff->readInt());
    mPauseCnt = buff->readLong();
    mActiveCnt = buff->readLong();
    mWalkingCnt = buff->readLong();

    int x = buff->readInt();
    int y = buff->readInt();
    setPosInMap(x, y);
}

void NPC::write(SaveLoadStream *buff)
{
    buff->writeInt(mType);
    buff->writeInt(mIndex);
    buff->writeInt(getCharacterState());
    buff->writeString(getName());
    buff->writeInt(mDelay);
    buff->writeInt(getWalkingSpriteId());
    buff->writeInt(getDirection());
    buff->writeInt(getStep());
    buff->writeLong(mPauseCnt);
    buff->writeLong(mActiveCnt);
    buff->writeLong(mWalkingCnt);
    buff->writeInt(getPosInMap().x);
    buff->writeInt(getPosInMap().y);
}

void NPC::setICanWalk(ICanWalk *arg)
{
    mCanWalk = arg;
}
