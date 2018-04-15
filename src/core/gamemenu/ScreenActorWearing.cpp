#include "ScreenActorWearing.h"
#include "ScreenMainGame.h"
#include "Util.h"
#include "Rect.h"
#include "GoodsEquipment.h"
#include "GameView.h"
#include "ScreenGoodsList.h"
#include "ScreenChgEquipment.h"
#include "GoodsEquipment.h"

Rect * ScreenActorWearing::sRectDesc = new Rect(9 + 3, 28 + 3, 151, 65);;

ScreenActorWearing::ScreenActorWearing()
{
    mActorIndex = -1;

    if (ScreenMainGame::sPlayerList.size() > 0)
    {
        GoodsEquipment **equipments = ScreenMainGame::sPlayerList.at(0)->getEquipmentsArray();
        for (int i = 0; i < 8; i++)
        {
            mEquipments[i] = equipments[i];
        }
        mActorIndex = 0;
    }

    mPos[0].set(4, 3);
    mPos[1].set(4, 30);
    mPos[2].set(21, 59);
    mPos[3].set(51, 65);
    mPos[4].set(80, 61);
    mPos[5].set(109, 46);
    mPos[6].set(107, 9);
    mPos[7].set(79, 2);

    mItemName[0] = "装饰";
    mItemName[1] = "装饰";
    mItemName[2] = "护腕";
    mItemName[3] = "脚蹬";
    mItemName[4] = "手持";
    mItemName[5] = "身穿";
    mItemName[6] = "肩披";
    mItemName[7] = "头戴";

    mCurItem = 0;
    showingDesc = false;

    bmpName = Util::getFrameBitmap(92 - 9 + 1, 29 - 10 + 1);
    bmpDesc = Util::getFrameBitmap(151 - 9 + 1, 65 - 28 + 1);

    mToDraw = 0; // 当前要画的描述中的字节
    mNextToDraw = 0; // 下一个要画的描述中的字节
}

ScreenActorWearing::~ScreenActorWearing()
{
    delete bmpName;
    delete bmpDesc;
}

void ScreenActorWearing::draw(Canvas *canvas)
{
    canvas->drawColor(Manager::sCOLOR_WHITE);
    canvas->drawBitmap(Util::bmpChuandai, 160 - Util::bmpChuandai->getWidth(), 0);

    // 画装备
    for (int i = 0; i < 8; i++)
    {
        if (mEquipments[i] != NULL)
        {
            mEquipments[i]->draw(canvas, mPos[i].x + 1, mPos[i].y + 1);
        }
    }
    canvas->drawRect(mPos[mCurItem].x, mPos[mCurItem].y, mPos[mCurItem].x + 25, mPos[mCurItem].y + 25, &(Util::sBlackPaint));
    TextRender::drawText(canvas, mItemName[mCurItem], 120, 80);

    // 画人物头像、姓名
    if (mActorIndex >= 0)
    {
        Player *p = ScreenMainGame::sPlayerList.at(mActorIndex);
        p->drawHead(canvas, 44, 12);
        TextRender::drawText(canvas, p->getName(), 30, 40);
    }

    if (showingDesc)
    {
        canvas->drawBitmap(bmpName, 9, 10);
        canvas->drawBitmap(bmpDesc, 9, 28);
        TextRender::drawText(canvas, mTextName, 9 + 3, 10 + 3);
        mNextToDraw = TextRender::drawText(canvas, mTextDesc, mToDraw, sRectDesc);
    }
}

void ScreenActorWearing::resetDesc()
{
    if (showingDesc)
    {
        showingDesc = false;
        mToDraw = mNextToDraw = 0;
        mStackLastToDraw.clear();
    }
}

void ScreenActorWearing::onKeyDown(int key)
{
    if (key == KEY_DOWN && mCurItem < 8 - 1)
    {
        ++mCurItem;
        resetDesc();
    }
    else if (key == KEY_UP && mCurItem > 0)
    {
        --mCurItem;
        resetDesc();
    }
    else if (key == KEY_RIGHT && mActorIndex < ScreenMainGame::sPlayerList.size() - 1)
    {
        ++mActorIndex;
        GoodsEquipment **equipments = ScreenMainGame::sPlayerList.at(mActorIndex)->getEquipmentsArray();
        for (int i = 0; i < 8; i++)
        {
            mEquipments[i] = equipments[i];
        }
        resetDesc();
    }
    else if (key == KEY_LEFT && mActorIndex > 0)
    {
        --mActorIndex;
        GoodsEquipment **equipments = ScreenMainGame::sPlayerList.at(mActorIndex)->getEquipmentsArray();
        for (int i = 0; i < 8; i++)
        {
            mEquipments[i] = equipments[i];
        }
        resetDesc();
    }
    else if (showingDesc)
    {
        if (key == KEY_PAGEDOWN)
        {
            if (mNextToDraw < mTextDesc.size())
            {
                mStackLastToDraw.push(mToDraw);
                mToDraw = mNextToDraw;
            }
        }
        else if (key == KEY_PAGEUP && mToDraw != 0)
        {
            if (!mStackLastToDraw.empty())
            {
                mToDraw = mStackLastToDraw.pop();
            }
        }
    }
}

void ScreenActorWearing::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        if (!showingDesc && mEquipments[mCurItem] != NULL)
        {
            showingDesc = true;
            mTextName = mEquipments[mCurItem]->getName();
            mTextDesc = mEquipments[mCurItem]->getDescription();
        }
        else
        {
            // put change equipment screen
            resetDesc();

            struct ListenerChgEquipment :public ScreenGoodsList::OnItemSelectedListener
            {
                int mActorIndex;
                ListenerChgEquipment(int actorIndex)
                    :mActorIndex(actorIndex)
                {

                }

                virtual void onItemSelected(BaseGoods *goods)
                {
                    Player *actor = ScreenMainGame::sPlayerList.at(mActorIndex);
                    if (goods->canPlayerUse(actor->getIndex()))
                    {
                        GameView::getInstance()->popScreen();
                        GameView::getInstance()->pushScreen(new ScreenChgEquipment(actor, static_cast<GoodsEquipment *>(goods)));
                    }
                    else
                    {
                        Util::showMessage("不能装备!", 1000);
                    }
                }
            };

            GameView::getInstance()->pushScreen(new ScreenGoodsList(
                getTheEquipList(Player::sEquipTypes[mCurItem]),
                new ListenerChgEquipment(mActorIndex),
                ScreenGoodsList::Use));
        }
    }
}

std::vector<BaseGoods*> *ScreenActorWearing::getTheEquipList(int type)
{
    vector<BaseGoods *> *list = Player::sGoodsList->getEquipList();
    static vector<BaseGoods*> *tmplist = NULL;
    if (tmplist == NULL)
    {
        tmplist = new vector < BaseGoods * >;
    }
    tmplist->clear();

    vector<BaseGoods*>::const_iterator iter = list->begin();
    for (; iter != list->end(); ++iter)
    {
        BaseGoods *g = *iter;
        if (g->getType() == Player::sEquipTypes[mCurItem])
        {
            // 找到所有与当前选择类型相同的装备
            tmplist->push_back(g);
        }
    }
    return tmplist;
}
