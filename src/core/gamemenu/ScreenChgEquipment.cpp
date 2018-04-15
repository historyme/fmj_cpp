#include "ScreenChgEquipment.h"
#include "Player.h"
#include "GoodsEquipment.h"
#include "GameView.h"

ScreenChgEquipment::ScreenChgEquipment(Player *actor, GoodsEquipment *goods)
    :mPage(0)
{
    mActor = actor;
    if (actor->hasSpace(goods->getType()))
    {
        mGoods.clear();
        mGoods.push_back(goods);
        mSelIndex = 0;
    }
    else
    {
        mGoods.clear();
        mGoods.push_back(actor->getCurrentEquipment(goods->getType()));
        mGoods.push_back(goods);
        mSelIndex = 1;
        // 没有空间，脱掉当前装备的
        actor->takeOff(goods->getType());
    }
    actor->putOn(goods);
}

void ScreenChgEquipment::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    if (mActor != NULL)
    {
        mActor->drawState(canvas, mPage);
        mActor->drawHead(canvas, 5, 60);
    }
    for (int i = 0; i < mGoods.size(); i++)
    {
        if (mGoods[i] != NULL)
        {
            mGoods[i]->draw(canvas, 8, 2 + 32 * i);
        }
    }
    Util::drawTriangleCursor(canvas, 1, 10 + 32 * mSelIndex);
}

void ScreenChgEquipment::onKeyDown(int key)
{
    if (key == KEY_UP && mSelIndex > 0)
    {
        mActor->takeOff(mGoods[mSelIndex]->getType());
        --mSelIndex;
        mActor->putOn(mGoods[mSelIndex]);
    }
    else if (key == KEY_DOWN && mSelIndex < mGoods.size() - 1)
    {
        mActor->takeOff(mGoods[mSelIndex]->getType());
        ++mSelIndex;
        mActor->putOn(mGoods[mSelIndex]);
    }
    else if (key == KEY_PAGEDOWN || key == KEY_PAGEUP)
    {
        mPage = 1 - mPage;
    }
}

void ScreenChgEquipment::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        // 换上原来的装备
        mActor->takeOff(mGoods[0]->getType());
        if (mGoods.size() > 1)
        {
            mActor->putOn(mGoods[0]);
        }
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        if (mSelIndex == mGoods.size() - 1)
        {
            // 换了新装备
            // 物品链中删除该装备
            Player::sGoodsList->deleteGoods(mGoods[mGoods.size() - 1]->getType(),
                mGoods[mGoods.size() - 1]->getIndex());
            // 物品链中加入老装备
            if (mGoods.size() > 1)
            {
                Player::sGoodsList->addGoods(mGoods[0]->getType(), mGoods[0]->getIndex());
            }
        }
        GameView::getInstance()->popScreen();
    }
}
