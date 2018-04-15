#include "CombatSuccess.h"
#include "Player.h"
#include "ResLevelupChain.h"
#include "ResImage.h"

CombatSuccess::CombatSuccess(int exp, int money, vector<BaseGoods*> goodslist, vector<Player*> lvuplist)
    :mIsAnyKeyPressed(false)
{
    mGoodsList = goodslist;
    mMsgList.clear();
    std::string e = int_to_string(exp);
    mMsgList.push_back(new MsgScreen(string("获得经验     ") + e, 18));
    std::string m = int_to_string(money); 
    mMsgList.push_back(new MsgScreen(string("战斗获得        ") + m + string("钱"), 46));

    mLvupList.clear();
    vector<Player *>::const_iterator iter = lvuplist.begin();
    for (; iter != lvuplist.end(); ++iter)
    {
        Player *p = *iter;
        mLvupList.push_back(new MsgScreen(p->getName() + string("修行提升")));
        mLvupList.push_back(new LevelupScreen(p));

        if (p->getLevelupChain()->getLearnMagicNum(p->getLevel()) >
            p->getLevelupChain()->getLearnMagicNum(p->getLevel() - 1))
        {
            mLvupList.push_back(new LearnMagicScreen(p->getName(),
                p->getMagicChain()->getMagic(p->getLevelupChain()->getLearnMagicNum(p->getLevel()) - 1)->getMagicName()));
        }
    }
}

CombatSuccess::~CombatSuccess()
{
    while (!mMsgList.empty())
    {
        delete mMsgList.back();
        mMsgList.pop_back();
    }

    while (!mLvupList.empty())
    {
        delete mLvupList.back();
        mLvupList.pop_back();
    }
}

bool CombatSuccess::update(long delta)
{
    mCnt += delta;
    if (mCnt > 1000 || mIsAnyKeyPressed)
    {
        mCnt = 0;
        mIsAnyKeyPressed = false;
        if (mGoodsList.size() == 0)
        {
            if (mLvupList.size() == 0)
            {
                return true;
            }
            else
            {
                mMsgList.push_back(mLvupList.back());
                mLvupList.pop_back();
            }
        }
        else
        {
            BaseGoods *g = mGoodsList.back();
            mGoodsList.pop_back();

            mMsgList.push_back(new MsgScreen(string("得到 ") + g->getName() + string(" x") + int_to_string(g->getGoodsNum())));

            delete g;
        }
    }
    return false;
}

void CombatSuccess::draw(Canvas *canvas)
{
    vector<BaseScreen *>::const_iterator iter = mMsgList.begin();
    for (; iter != mMsgList.end(); ++iter)
    { 
        BaseScreen *s = *iter;
        s->draw(canvas);
    }
}

MsgScreen::MsgScreen(std::string _msg, int y)
{
    string msg = _msg;

    mSide = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 2, 8);
    mMsg = Bitmap::createBitmap(msg.size() * 8 + 8, 24, ARGB_8888);
    Canvas *c = new Canvas(mMsg);
    c->drawColor(Manager::sCOLOR_WHITE);
    mSide->draw(c, 1, 0, 0);
    mSide->draw(c, 2, mMsg->getWidth() - 3, 0);
    Paint *p = new Paint();
    p->setColor(Manager::sCOLOR_BLACK);
    p->setStyle(Paint::FILL_AND_STROKE);
    c->drawLine(0, 1, mMsg->getWidth(), 1, p);
    c->drawLine(0, 22, mMsg->getWidth(), 22, p);
    TextRender::drawText(c, msg, 4, 4);

    mX = (160 - mMsg->getWidth()) / 2;
    mY = y;

    delete c;
    delete p;
}

MsgScreen::~MsgScreen()
{
    delete mMsg;
    delete mSide;
}

void MsgScreen::draw(Canvas *canvas)
{
    canvas->drawBitmap(mMsg, mX, mY);
}

LevelupScreen::LevelupScreen(Player *p)
{
    mSide = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 2, 9);
    mInfo = mSide->getBitmap(0);

    Canvas *canvas = new Canvas(mInfo);
    ResLevelupChain *lc = p->getLevelupChain();
    int curl = p->getLevel();

    Util::drawSmallNum(canvas, p->getHP(), 37, 9);
    p->setHP(p->getMaxHP());
    Util::drawSmallNum(canvas, p->getMaxHP() - (lc->getMaxHP(curl) - lc->getMaxHP(curl - 1)), 56, 9);
    Util::drawSmallNum(canvas, p->getMaxHP(), 86, 9);
    Util::drawSmallNum(canvas, p->getMaxHP(), 105, 9);
    Util::drawSmallNum(canvas, p->getMP(), 37, 21);
    p->setMP(p->getMaxMP());
    Util::drawSmallNum(canvas, p->getMaxMP() - (lc->getMaxMP(curl) - lc->getMaxMP(curl - 1)), 56, 21);
    Util::drawSmallNum(canvas, p->getMaxMP(), 86, 21);
    Util::drawSmallNum(canvas, p->getMaxMP(), 105, 21);
    Util::drawSmallNum(canvas, p->getAttack() - (lc->getAttack(curl) - lc->getAttack(curl - 1)), 47, 33);
    Util::drawSmallNum(canvas, p->getAttack(), 96, 33);
    Util::drawSmallNum(canvas, p->getDefend() - (lc->getDefend(curl) - lc->getDefend(curl - 1)), 47, 45);
    Util::drawSmallNum(canvas, p->getDefend(), 96, 45);
    Util::drawSmallNum(canvas, p->getSpeed() - (lc->getSpeed(curl) - lc->getSpeed(curl - 1)), 47, 57);
    Util::drawSmallNum(canvas, p->getSpeed(), 96, 57);
    Util::drawSmallNum(canvas, p->getLingli() - (lc->getLingli(curl) - lc->getLingli(curl - 1)), 47, 69);
    Util::drawSmallNum(canvas, p->getLingli(), 96, 69);
    Util::drawSmallNum(canvas, p->getLuck() - (lc->getLuck(curl) - lc->getLuck(curl - 1)), 47, 81);
    Util::drawSmallNum(canvas, p->getLuck(), 96, 81);

    delete canvas;
}

LevelupScreen::~LevelupScreen()
{
    delete mSide;
}

void LevelupScreen::draw(Canvas *canvas)
{
    canvas->drawBitmap(mInfo, (160 - mInfo->getWidth()) / 2, (96 - mInfo->getHeight()) / 2);
}

LearnMagicScreen::LearnMagicScreen(std::string playerName, string magicName)
{
    mSide = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 2, 10);
    mInfo = mSide->getBitmap(0);

    Canvas *canvas = new Canvas(mInfo);
    TextRender::drawText(canvas, playerName, (mInfo->getWidth() - playerName.size() * 8) / 2, 8);
    TextRender::drawText(canvas, magicName, (mInfo->getWidth() - magicName.size() * 8) / 2, 42);

    delete canvas;
}

LearnMagicScreen::~LearnMagicScreen()
{
    delete mSide;
}

void LearnMagicScreen::draw(Canvas *canvas)
{
    canvas->drawBitmap(mInfo, (160 - mInfo->getWidth()) / 2, (96 - mInfo->getHeight()) / 2);
}
