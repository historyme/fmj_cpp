#include "RaiseAnimation.h"


RaiseAnimation::RaiseAnimation(int x, int y, int hitpoint, int buff)
{
    cnt = 0;
    dy = 0;
    dt = 0;

    this->x = x;
    this->y = y;
    raiseNum = Util::getSmallSignedNumBitmap(hitpoint);
    this->bShowNum = hitpoint != 0;
    srsList.clear();

    if ((buff & FightingCharacter::BUFF_MASK_DU) == FightingCharacter::BUFF_MASK_DU)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 243));
        srsList.back()->startAni();
    }
    if ((buff & FightingCharacter::BUFF_MASK_LUAN) == FightingCharacter::BUFF_MASK_LUAN)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 244));
        srsList.back()->startAni();
    }
    if ((buff & FightingCharacter::BUFF_MASK_FENG) == FightingCharacter::BUFF_MASK_FENG)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 245));
        srsList.back()->startAni();
    }
    if ((buff & FightingCharacter::BUFF_MASK_MIAN) == FightingCharacter::BUFF_MASK_MIAN)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 246));
        srsList.back()->startAni();
    }
    if ((buff & FightingCharacter::BUFF_MASK_GONG) == FightingCharacter::BUFF_MASK_GONG)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 240));
        srsList.back()->startAni();
    }
    if ((buff & FightingCharacter::BUFF_MASK_FANG) == FightingCharacter::BUFF_MASK_FANG)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 241));
        srsList.back()->startAni();
    }
    if ((buff & FightingCharacter::BUFF_MASK_SU) == FightingCharacter::BUFF_MASK_SU)
    {
        srsList.push_back((ResSrs *)DatLib::GetRes(DatLib::RES_SRS, 1, 242));
        srsList.back()->startAni();
    }
}

RaiseAnimation::~RaiseAnimation()
{
    delete raiseNum;
}

bool RaiseAnimation::update(long delta)
{
    if (bShowNum)
    {
        cnt += delta;
        if (cnt > 50)
        {
            cnt = 0;
            ++dt;
            dy -= dt;
            if (dt > 4)
            {
                bShowNum = false;
            }
        }
    }
    else
    {
        if (srsList.empty())
        {
            return false;
        }
        else
        {
            if (!srsList.front()->update(delta))
            {
                delete srsList.front();
                srsList.pop_front();
                return !srsList.empty();
            }
        }
    }
    return true;
}

void RaiseAnimation::draw(Canvas *canvas)
{
    if (bShowNum)
    {
        canvas->drawBitmap(raiseNum, x, y + dy);
    }
    else
    {
        if (srsList.size() > 0)
        {
            srsList.front()->drawAbsolutely(canvas, x, y);
        }
    }
}
