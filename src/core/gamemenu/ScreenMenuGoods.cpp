#include "ScreenMenuGoods.h"
#include "Player.h"
#include "ScreenMainGame.h"
#include "ScreenTakeMedicine.h"
#include "ScreenChgEquipment.h"

ScreenMenuGoods::ScreenMenuGoods()
{
    strs[0] = "使用";
    strs[1] = "装备";

    mSelId = 0;

    mFrameBmp = Util::getFrameBitmap(77 - 39 + 1, 77 - 39 + 1);
}

ScreenMenuGoods::~ScreenMenuGoods()
{
    delete mFrameBmp;
}

void ScreenMenuGoods::draw(Canvas *canvas)
{
    canvas->drawBitmap(mFrameBmp, 39, 39);
    if (mSelId == 0)
    {
        TextRender::drawSelText(canvas, strs[0], 39 + 3, 39 + 3);
        TextRender::drawText(canvas, strs[1], 39 + 3, 39 + 3 + 16);
    }
    else if (mSelId == 1)
    {
        TextRender::drawText(canvas, strs[0], 39 + 3, 39 + 3);
        TextRender::drawSelText(canvas, strs[1], 39 + 3, 39 + 3 + 16);
    }
}

void ScreenMenuGoods::onKeyDown(int key)
{
    if (key == KEY_UP || key == KEY_DOWN)
    {
        mSelId = 1 - mSelId;
    }
}

struct ScreenMenuGoodsListener : public ScreenGoodsList::OnItemSelectedListener
{
public:
    int mSelId;
    ScreenMenuGoodsListener(int selId);
    virtual void onItemSelected(BaseGoods *goods);

private:
    void goodsSelected(BaseGoods *goods);
    void equipSelected(BaseGoods *goods);
};

void ScreenMenuGoods::onKeyUp(int key)
{
    int selId = mSelId; //popScreen删掉自己了，需要在堆栈中保存

    if (key == KEY_CANCEL)
    {
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_ENTER)
    {
        GameView::getInstance()->popScreen();
        GameView::getInstance()->pushScreen(new ScreenGoodsList(
            selId == 0 ? Player::sGoodsList->getGoodsList() : Player::sGoodsList->getEquipList(),
            new ScreenMenuGoodsListener(selId),
            ScreenGoodsList::Use));
    }
}

ScreenMenuGoodsListener::ScreenMenuGoodsListener(int selId)
    :mSelId(selId)
{

}

void ScreenMenuGoodsListener::onItemSelected(BaseGoods *goods)
{
    if (mSelId == 0)
    {
        // 使用
        goodsSelected(goods);
    }
    else if (mSelId == 1)
    {
        // 装备
        equipSelected(goods);
    }
}

void ScreenMenuGoodsListener::goodsSelected(BaseGoods *goods)
{
    switch (goods->getType())
    {
    case 8: // 暗器
    case 12: // 兴奋剂
        Util::showMessage("战斗中才能使用!", 1000);
        break;

    case 13: // 土遁
        // TODO 迷宫中的用法，调用脚本
        ScreenMainGame::triggerEvent(255);
        while (!(GameView::getInstance()->getCurScreen()->bInstanceof_ScreenMainGame))
        {
            GameView::getInstance()->popScreen();
        }
        break;

    case 14: // 剧情类
        // TODO 剧情类物品用法
        Util::showMessage("当前无法使用!", 1000);
        break;

    case 9: // 药物
    case 10: // 灵药
    case 11: // 仙药
        GameView::getInstance()->pushScreen(new ScreenTakeMedicine(goods));
        break;
    }
}

void ScreenMenuGoodsListener::equipSelected(BaseGoods *goods)
{
    vector<Player*> list;
    for (int i = 0; i < ScreenMainGame::sPlayerList.size(); i++)
    {
        Player *p = ScreenMainGame::sPlayerList.at(i);
        if (goods->canPlayerUse(p->getIndex()))
        {
            list.push_back(p);
        }
    }
    if (list.size() == 0)
    {
        // 没人能装备
        Util::showMessage("不能装备!", 1000);
    }
    else if (list.size() == 1)
    {
        // 一个人能装备
        if (list.at(0)->hasEquipt(goods->getType(), goods->getIndex()))
        {
            Util::showMessage("已装备!", 1000);
        }
        else
        {
            GameView::getInstance()->pushScreen(new ScreenChgEquipment(list.at(0), (GoodsEquipment*)goods));
        }
    }
    else
    {
        // 多人可装备
        class MutilPlayerEquip : public BaseScreen
        {
        public:
            Bitmap *bg;
            int curSel;
            string *itemsText;
            vector<Player*> list;
            BaseGoods *goods;

            MutilPlayerEquip(vector<Player*> list, BaseGoods *goods)
            {
                this->list = list;
                this->goods = goods;

                bg = Util::getFrameBitmap(16 * 5 + 6, 6 + 16 * list.size());
                itemsText = new string[list.size()];

                for (int i = 0; i < list.size(); i++)
                {
                    itemsText[i] = list.at(i)->getName();
                }

                curSel = 0;
            }

            virtual void update(long delta)
            {
            }

            virtual void  onKeyUp(int key)
            {
                if (key == KEY_ENTER)
                {
                    if (list.at(curSel)->hasEquipt(goods->getType(), goods->getIndex()))
                    {
                        Util::showMessage("已装备!", 1000);
                    }
                    else
                    {
                        GameView::getInstance()->popScreen();
                        GameView::getInstance()->pushScreen(new ScreenChgEquipment(list.at(curSel), (GoodsEquipment *)goods));
                    }
                }
                else if (key == KEY_CANCEL)
                {
                    GameView::getInstance()->popScreen();
                }
            }

            virtual void  onKeyDown(int key)
            {
                if (key == KEY_DOWN)
                {
                    ++curSel;
                    curSel %= list.size();
                }
                else if (key == KEY_UP)
                {
                    --curSel;
                    curSel = (curSel + list.size()) % list.size();
                }
            }

            virtual void draw(Canvas *canvas)
            {
                canvas->drawBitmap(bg, 50, 14);
                for (int i = 0; i < list.size(); i++)
                {
                    if (i != curSel)
                    {
                        TextRender::drawText(canvas, itemsText[i], 50 + 3, 14 + 3 + 16 * i);
                    }
                    else
                    {
                        TextRender::drawSelText(canvas, itemsText[i], 50 + 3, 14 + 3 + 16 * i);
                    }
                }
            }
            virtual bool isPopup()
            {
                return true;
            }
        };

        GameView::getInstance()->pushScreen(new MutilPlayerEquip(list, goods));

    }
}
