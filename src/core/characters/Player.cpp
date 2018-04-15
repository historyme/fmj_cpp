#include "Player.h"
#include "GoodsManage.h"
#include "common.h"

#include "Character.h"
#include "Direction.h"
#include "Random.h"

#include "TextRender.h"
#include "Util.h"
#include "DatLib.h"
#include "ResImage.h"
#include "ResMagicChain.h"
#include "ResLevelupChain.h"
#include "Canvas.h"
#include "GoodsEquipment.h"
#include "SaveLoadStream.h"

Player::Player()
{
    bInstanceof_Player = true;

    mImgHead = NULL;
    mLevelupChain = NULL;

    for (int i = 0; i < 8; i++)
    {
        mEquipments[i] = NULL;
    }
}

Player::~Player()
{
    if (NULL != mImgHead)
    {
        delete mImgHead;
    }

    if (NULL != mLevelupChain)
    {
        delete mLevelupChain;
    }

    for (int i = 0; i < 8; i++)
    {
        if (NULL != mEquipments[i])
        {
            delete mEquipments[i];
        }
    }
}

void Player::drawHead(Canvas *canvas, int x, int y)
{
    if (mImgHead != NULL)
    {
        mImgHead->draw(canvas, 1, x, y);
    }
}

void Player::setFrameByState()
{
    if (isAlive())
    {
        if (hasDebuff(BUFF_MASK_MIAN) || getHP() < getMaxHP() / 10)
        {
            getFightingSprite()->setCurrentFrame(11);//跪着的状态
        }
        else
        {
            getFightingSprite()->setCurrentFrame(1);
        }
    }
    else
    {
        getFightingSprite()->setCurrentFrame(12);//站着的状态
    }
}

void Player::setData(char *buf, int offset)
{
    mType = (int)buf[offset] & 0xFF;
    mIndex = (int)buf[offset + 1] & 0xFF;
    mImgHead = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 1, mIndex);
    setWalkingSprite(new WalkingSprite(mType, (int)buf[offset + 0x16] & 0xFF));
    setFightingSprite(new FightingSprite(DatLib::RES_PIC, mIndex));
    Direction d = North;
    switch ((int)buf[offset + 2] & 0xFF)
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
    setStep((int)buf[offset + 3] & 0xff);
    setPosInMap((int)buf[offset + 5] & 0xFF, (int)buf[offset + 6] & 0xFF);
    setMagicChain((ResMagicChain *)DatLib::GetRes(DatLib::RES_MLR, 1, (int)buf[offset + 0x17] & 0xff));
    getMagicChain()->setLearnNum((int)buf[offset + 9] & 0xff);
    setName(getString(buf, offset + 0x0a));
    setLevel((int)buf[offset + 0x20] & 0xff);
    setMaxHP(get2BytesInt(buf, offset + 0x26));
    setHP(get2BytesInt(buf, offset + 0x28));
    setMaxMP(get2BytesInt(buf, offset + 0x2a));
    setMP(get2BytesInt(buf, offset + 0x2c));
    setAttack(get2BytesInt(buf, offset + 0x2e));
    setDefend(get2BytesInt(buf, offset + 0x30));
    setSpeed((int)buf[offset + 0x36] & 0xff);
    setLingli((int)buf[offset + 0x37] & 0xff);
    setLuck((int)buf[offset + 0x38] & 0xff);
    setCurrentExp(get2BytesInt(buf, offset + 0x32));
    mLevelupChain = (ResLevelupChain *)DatLib::GetRes(DatLib::RES_MLR, 2, mIndex);

    int tmp;

    tmp = (int)buf[offset + 0x1e] & 0xff;
    if (tmp != 0)
    {
        mEquipments[0] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 6, tmp);
    }

    tmp = (int)buf[offset + 0x1f] & 0xff;
    if (tmp != 0)
    {
        mEquipments[1] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 6, tmp);
    }

    tmp = (int)buf[offset + 0x1b] & 0xff;
    if (tmp != 0)
    {
        mEquipments[2] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 5, tmp);
    }

    tmp = (int)buf[offset + 0x1d] & 0xff;
    if (tmp != 0)
    {
        mEquipments[3] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 3, tmp);
    }

    tmp = (int)buf[offset + 0x1c] & 0xff;
    if (tmp != 0)
    {
        mEquipments[4] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 7, tmp);
    }

    tmp = (int)buf[offset + 0x19] & 0xff;
    if (tmp != 0)
    {
        mEquipments[5] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 2, tmp);
    }

    tmp = (int)buf[offset + 0x1a] & 0xff;
    if (tmp != 0)
    {
        mEquipments[6] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 4, tmp);
    }

    tmp = (int)buf[offset + 0x18] & 0xff;
    if (tmp != 0)
    {
        mEquipments[7] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, 1, tmp);
    }
}

GoodsEquipment * Player::getCurrentEquipment(int type)
{
    for (int i = 0; i < 8; i++)
    {
        if (sEquipTypes[i] == type)
        {
            return mEquipments[i];
        }
    }
    return NULL;
}

GoodsEquipment ** Player::getEquipmentsArray()
{
    return mEquipments;
}

bool Player::hasEquipt(int type, int id)
{
    if (type == 6)
    {
        // 两个位置都装备同一件装备才返回真
        if ((mEquipments[0] != NULL && mEquipments[0]->getType() == type && mEquipments[0]->getIndex() == id) &&
            (mEquipments[1] != NULL && mEquipments[1]->getType() == type && mEquipments[1]->getIndex() == id))
        {
            return true;
        }
        return false;
    }

    for (int i = 2; i < 8; i++)
    {
        if (mEquipments[i] != NULL && mEquipments[i]->getType() == type
            && mEquipments[i]->getIndex() == id)
        {
            return true;
        }
    }
    return false;
}

void Player::putOn(GoodsEquipment *goods)
{
    for (int i = 0; i < 8; i++)
    {
        if (goods->getType() == sEquipTypes[i])
        {
            if (mEquipments[i] == NULL)   // 适用2个装饰
            {
                goods->putOn(this);
                mEquipments[i] = goods;
                break;
            }
        }
    }
}

void Player::takeOff(int type)
{
    for (int i = 0; i < 8; i++)
    {
        if (type == sEquipTypes[i])
        {
            if (mEquipments[i] != NULL)
            {
                mEquipments[i]->takeOff(this);
                mEquipments[i] = NULL;
                break;
            }
        }
    }
}

bool Player::hasSpace(int type)
{
    if (type == 6)   // 饰品
    {
        if (mEquipments[0] == NULL || mEquipments[1] == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            if (sEquipTypes[i] == type && mEquipments[i] == NULL)
            {
                return true;
            }
        }
    }
    return false;
}

void Player::drawState(Canvas *canvas, int page)
{
    canvas->drawLine(37, 10, 37, 87, &Util::sBlackPaint);
    if (page == 0)
    {
        TextRender::drawText(canvas, std::string("等级   ") + int_to_string(getLevel()), 41, 4);
        TextRender::drawText(canvas, std::string("生命   ") + int_to_string(getHP()) + std::string("/") + int_to_string(getMaxHP()), 41, 23);
        TextRender::drawText(canvas, std::string("真气   ") + int_to_string(getMP()) + std::string("/") + int_to_string(getMaxMP()), 41, 41);
        TextRender::drawText(canvas, std::string("攻击力 ") + int_to_string(getAttack()), 41, 59);
        TextRender::drawText(canvas, std::string("防御力 ") + int_to_string(getDefend()), 41, 77);
    }
    else if (page == 1)
    {
        TextRender::drawText(canvas, std::string("经验值"), 41, 4);
        int w = Util::drawSmallNum(canvas, getCurrentExp(), 97, 4);
        TextRender::drawText(canvas, "/", 97 + w + 2, 4);
        Util::drawSmallNum(canvas, getLevelupChain()->getNextLevelExp(getLevel()), 97 + w + 9, 10);
        TextRender::drawText(canvas, std::string("身法   ") + int_to_string(getSpeed()), 41, 23);
        TextRender::drawText(canvas, std::string("灵力   ") + int_to_string(getLingli()), 41, 41);
        TextRender::drawText(canvas, std::string("幸运   ") + int_to_string(getLuck()), 41, 59);

        std::string sb("免疫   ");
        std::string tmp("");
        if (hasBuff(BUFF_MASK_DU))
        {
            tmp += "毒";
        }
        if (hasBuff(BUFF_MASK_LUAN))
        {
            tmp += "乱";
        }
        if (hasBuff(BUFF_MASK_FENG))
        {
            tmp += "封";
        }
        if (hasBuff(BUFF_MASK_MIAN))
        {
            tmp += "眠";
        }
        if (tmp.length() > 0)
        {
            sb.append(tmp);
        }
        else
        {
            sb.append("无");
        }
        TextRender::drawText(canvas, sb, 41, 77);
    }
}

ResLevelupChain * Player::getLevelupChain()
{
    return mLevelupChain;
}

void Player::setCurrentExp(int exp)
{
    mCurrentExp = exp;
}

const int Player::sEquipTypes[8] = { 6, 6, 5, 3, 7, 2, 4, 1 };
GoodsManage * Player::sGoodsList = new GoodsManage();;
int Player::sMoney = 0;

void Player::read(SaveLoadStream *buff)
{
    mType = buff->readInt();
    mIndex = buff->readInt();
    mImgHead = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 1, mIndex);
    mLevelupChain = (ResLevelupChain*)DatLib::GetRes(DatLib::RES_MLR, 2, mIndex);

    setWalkingSprite(new WalkingSprite(mType, buff->readInt()));
    setFightingSprite(new FightingSprite(DatLib::RES_PIC, mIndex));

    int d = buff->readInt();
    setDirection((Direction)d);

    setStep(buff->readInt());

    int x = buff->readInt();
    int y = buff->readInt();
    setPosInMap(x, y);

    setMagicChain((ResMagicChain*)DatLib::GetRes(DatLib::RES_MLR, 1, buff->readInt()));
    getMagicChain()->setLearnNum(buff->readInt());
    setName(buff->readString());
    setLevel(buff->readInt());
    setMaxHP(buff->readInt());
    setHP(buff->readInt());
    setMaxMP(buff->readInt());
    setMP(buff->readInt());
    setAttack(buff->readInt());
    setDefend(buff->readInt());
    setSpeed(buff->readInt());
    setLingli(buff->readInt());
    setLuck(buff->readInt());
    setCurrentExp(buff->readInt());
    for (int i = 0; i < 8; i++)
    {
        int type = buff->readInt();
        int index = buff->readInt();
        if (type != 0 && index != 0)
        {
            mEquipments[i] = (GoodsEquipment *)DatLib::GetRes(DatLib::RES_GRS, type, index);
        }
    }
}

void Player::write(SaveLoadStream *buff)
{
    buff->writeInt(mType);
    buff->writeInt(mIndex);
    buff->writeInt(getWalkingSpriteId());
    buff->writeInt(getDirection());
    buff->writeInt(getStep());
    buff->writeInt(getPosInMap().x);
    buff->writeInt(getPosInMap().y);
    buff->writeInt(getMagicChain()->getIndex());
    buff->writeInt(getMagicChain()->getLearnNum());
    buff->writeString(getName());
    buff->writeInt(getLevel());
    buff->writeInt(getMaxHP());
    buff->writeInt(getHP());
    buff->writeInt(getMaxMP());
    buff->writeInt(getMP());
    buff->writeInt(getAttack());
    buff->writeInt(getDefend());
    buff->writeInt(getSpeed());
    buff->writeInt(getLingli());
    buff->writeInt(getLuck());
    buff->writeInt(getCurrentExp());
    for (int i = 0; i < 8; i++)
    {
        if (mEquipments[i] == NULL)
        {
            buff->writeInt(0);
            buff->writeInt(0);
        }
        else
        {
            buff->writeInt(mEquipments[i]->getType());
            buff->writeInt(mEquipments[i]->getIndex());
        }
    }
}
