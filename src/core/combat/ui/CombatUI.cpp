#include "CombatUI.h"
#include "GameView.h"
#include "MagicAttack.h"
#include "ResImage.h"
#include "DatLib.h"
#include "ScreenMagic.h"
#include "ScreenGoodsList.h"
#include "FrameAnimation.h"
#include "ActionPhysicalAttackAll.h"
#include "ActionMultiTarget.h"
#include "ActionSingleTarget.h"
#include "ActionPhysicalAttackOne.h"
#include "ActionMagicAttackAll.h"
#include "ActionMagicAttackOne.h"
#include "ActionMagicHelpAll.h"
#include "ActionDefend.h"
#include "ScreenChgEquipment.h"
#include "ActionMagicHelpOne.h"
#include "ActionCoopMagic.h"
#include "GoodsHiddenWeapon.h"
#include "ActionThrowItemAll.h"
#include "ActionThrowItemOne.h"
#include "ActionUseItemAll.h"
#include "ActionUseItemOne.h"

CombatUI * CombatUI::getInstance()
{
    static CombatUI instance;
    return &instance;
}

CombatUI::CombatUI()
{
    mHeadsImg[0] = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 1, 1);
    mHeadsImg[1] = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 1, 2);
    mHeadsImg[2] = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 1, 3);

    mPlayerIndicatorPos[0].set(69, 45);
    mPlayerIndicatorPos[1].set(101, 41);
    mPlayerIndicatorPos[2].set(133, 33);

    mMonsterIndicatorPos[0].set(16, 14);
    mMonsterIndicatorPos[1].set(48, 3);
    mMonsterIndicatorPos[2].set(86, 0);

    mScreenStack.clear();
    mScreenStack.push_back(new MainMenu());

    ResImage *tmpImg;
    tmpImg = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 2, 4);
    mPlayerIndicator = new FrameAnimation(tmpImg, 1, 2);
    mTargetIndicator = new FrameAnimation(tmpImg, 3, 4);
    tmpImg = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 2, 3);
    mMonsterIndicator = new FrameAnimation(tmpImg);
}

void CombatUI::update(long delta)
{
    std::vector<BaseScreen *>::const_iterator iter;
    for (iter = mScreenStack.begin(); iter != mScreenStack.end(); ++iter)
    {
        BaseScreen *i = *iter;

        i->update(delta);
    }
}

void CombatUI::draw(Canvas *canvas)
{
    std::vector<BaseScreen *>::const_iterator iter;
    for (iter = mScreenStack.begin(); iter != mScreenStack.end(); ++iter)
    {
        BaseScreen *i = *iter;

        i->draw(canvas);
    }
}

void CombatUI::onKeyDown(int key)
{
    BaseScreen *bs = mScreenStack.back();
    if (bs != NULL)
    {
        bs->onKeyDown(key);
    }
}

void CombatUI::onKeyUp(int key)
{
    BaseScreen *bs = mScreenStack.back();
    if (bs != NULL)
    {
        bs->onKeyUp(key);
    }
}

void CombatUI::reset()
{
    for (int i = 0; i < (int)(mScreenStack.size()); i++)
    {
        delete mScreenStack.at(i);
    }
    mScreenStack.clear();
    mScreenStack.push_back(new MainMenu());
}

void CombatUI::setPlayerList(std::vector<Player *> list)
{
    mPlayerList.clear();
    std::vector<Player *>::const_iterator iter;
    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        Player *i = *iter;

        mPlayerList.push_back(i);
    }
}

void CombatUI::setMonsterList(std::vector<Monster *> list)
{
    mMonsterList.clear();

    std::vector<Monster *>::const_iterator iter;
    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        FightingCharacter *i = *iter;

        mMonsterList.push_back(i);
    }
}

void CombatUI::setCurrentPlayerIndex(int i)
{
    mCurPlayerIndex = i;
}

void CombatUI::setCallBack(CallBack *callBack)
{
    mCallBack = callBack;
}

Player * CombatUI::getCurPlayer() const
{
    return static_cast<Player*>(mPlayerList.at(mCurPlayerIndex));
}

void CombatUI::onActionSelected(Action *action)
{
    if (mCallBack != NULL)
    {
        mCallBack->onActionSelected(action);
    }
}

void CombatUI::onCancel()
{
    if (mCallBack != NULL)
    {
        mCallBack->onCancel();
    }
}

MainMenu::MainMenu()
{
    mMenuIcon = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 2, 1);
    mPlayerInfoBg = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 2, 2);
    mCurIconIndex = 1;
}

MainMenu::~MainMenu()
{
    delete mMenuIcon;
    delete mPlayerInfoBg;
}

void MainMenu::update(long delta)
{
    CombatUI::getInstance()->mPlayerIndicator->update(delta);
}

void MainMenu::draw(Canvas *canvas)
{
    mMenuIcon->draw(canvas, mCurIconIndex, 7, 96 - mMenuIcon->getHeight());
    mPlayerInfoBg->draw(canvas, 1, 49, 66);
    Player *p = CombatUI::getInstance()->getCurPlayer();
    CombatUI::getInstance()->mHeadsImg[p->getIndex() - 1]->draw(canvas, 1, 50, 63); // 角色头像
    if (p != NULL)
    {
        Util::drawSmallNum(canvas, p->getHP(), 79, 72); // hp
        Util::drawSmallNum(canvas, p->getMaxHP(), 104, 72); // maxhp
        Util::drawSmallNum(canvas, p->getMP(), 79, 83); // mp
        Util::drawSmallNum(canvas, p->getMaxMP(), 104, 83); // maxmp
    }
    CombatUI::getInstance()->mPlayerIndicator->draw(canvas, 
        CombatUI::getInstance()->mPlayerIndicatorPos[CombatUI::getInstance()->mCurPlayerIndex].x, 
        CombatUI::getInstance()->mPlayerIndicatorPos[CombatUI::getInstance()->mCurPlayerIndex].y);
}

void MainMenu::onKeyDown(int key)
{
    switch (key)
    {
    case KEY_LEFT:
        if (CombatUI::getInstance()->getCurPlayer()->hasDebuff(Player::BUFF_MASK_FENG))
        {
            break; // 被封，不能用魔法
        }
        mCurIconIndex = 2;
        break;

    case KEY_DOWN:
        mCurIconIndex = 3;
        break;

    case KEY_RIGHT:
        if (CombatUI::getInstance()->mPlayerList.size() <= 1)   // 只有一人不能合击
        {
            break;
        }
        mCurIconIndex = 4;
        break;

    case KEY_UP:
        mCurIconIndex = 1;
        break;
    }
}

void MainMenu::onKeyUp(int key)
{
    if (key == KEY_ENTER)
    {
        switch (mCurIconIndex)
        {
        case 1://物理攻击
            // 攻击全体敌人
            if (CombatUI::getInstance()->getCurPlayer()->hasAtbuff(Player::BUFF_MASK_ALL))
            {
                CombatUI::getInstance()->onActionSelected(
                    new ActionPhysicalAttackAll(CombatUI::getInstance()->getCurPlayer(), 
                    CombatUI::getInstance()->mPlayerList));
                break;
            }

            struct ListenerPhysicalAttackOne : public OnCharacterSelectedListener
            {
                virtual void onCharacterSelected(FightingCharacter *fc)
                {
                    CombatUI::getInstance()->onActionSelected(new ActionPhysicalAttackOne(CombatUI::getInstance()->getCurPlayer(), fc));
                }
            };

            // 攻击单个敌人
            CombatUI::getInstance()->mScreenStack.push_back(
                new MenuCharacterSelect(CombatUI::getInstance()->mMonsterIndicator, 
                CombatUI::getInstance()->mMonsterIndicatorPos,
                CombatUI::getInstance()->mMonsterList, 
                new ListenerPhysicalAttackOne(), 
                true));
            break;

        case 2://魔法技能

            struct MagicItemSelected : public OnItemSelectedListener
            {
                virtual void onItemSelected(BaseMagic *magic)
                {
                    GameView::getInstance()->popScreen(); // 弹出魔法选择界面
                    if ((magic->mbInstanceof_MagicAttack) || (magic->mbInstanceof_MagicSpecial))
                    {
                        // 选一个敌人
                        if (magic->isForAll())
                        {
                            CombatUI::getInstance()->onActionSelected(new ActionMagicAttackAll(
                                CombatUI::getInstance()->getCurPlayer(),
                                CombatUI::getInstance()->mMonsterList,
                                (MagicAttack *)magic));
                        }
                        else
                        {
                            struct ListenerMagicAttackOne : public OnCharacterSelectedListener
                            {
                                BaseMagic *magic;
                                ListenerMagicAttackOne(BaseMagic *bm)
                                    :magic(bm)
                                {
                                }
                                virtual void onCharacterSelected(FightingCharacter *fc)
                                {
                                    CombatUI::getInstance()->onActionSelected(new ActionMagicAttackOne(
                                        CombatUI::getInstance()->getCurPlayer(),
                                        fc,
                                        (MagicAttack *)magic));
                                }
                            };

                            // 选一个敌人
                            CombatUI::getInstance()->mScreenStack.push_back(new MenuCharacterSelect(
                                CombatUI::getInstance()->mMonsterIndicator, 
                                CombatUI::getInstance()->mMonsterIndicatorPos,
                                CombatUI::getInstance()->mMonsterList, 
                                new ListenerMagicAttackOne(magic),
                                true));
                        }
                    }
                    else
                    {
                        // 选队友或自己
                        if (magic->isForAll())
                        {
                            CombatUI::getInstance()->onActionSelected(new ActionMagicHelpAll(
                                CombatUI::getInstance()->getCurPlayer(),
                                CombatUI::getInstance()->mPlayerList,
                                magic));
                        }
                        else
                        {
                            struct ListenerHelpOne : public OnCharacterSelectedListener
                            {
                                BaseMagic *magic;
                                ListenerHelpOne(BaseMagic *bm)
                                    :magic(bm)
                                {
                                }
                                virtual void onCharacterSelected(FightingCharacter *fc)
                                {
                                    CombatUI::getInstance()->onActionSelected(new ActionMagicHelpOne(
                                        CombatUI::getInstance()->getCurPlayer(),
                                        fc,
                                        magic));
                                }
                            };

                            // 选一个Player
                            CombatUI::getInstance()->mScreenStack.push_back(new MenuCharacterSelect(
                                CombatUI::getInstance()->mTargetIndicator,
                                CombatUI::getInstance()->mPlayerIndicatorPos,
                                CombatUI::getInstance()->mPlayerList,
                                new ListenerHelpOne(magic),
                                false));
                        }
                    }
                }
            };

            GameView::getInstance()->pushScreen(new ScreenMagic(
                CombatUI::getInstance()->getCurPlayer()->getMagicChain(),
                new MagicItemSelected()));
            break;

        case 3://杂项
            CombatUI::getInstance()->mScreenStack.push_back(new MenuMisc());
            break;

        case 4://合击
            struct ListenerCoopMagic : public OnCharacterSelectedListener
            {

                virtual void onCharacterSelected(FightingCharacter *fc)
                {
                    CombatUI::getInstance()->onActionSelected(new ActionCoopMagic(
                        CombatUI::getInstance()->mPlayerList,
                        fc));
                }
            };

            CombatUI::getInstance()->mScreenStack.push_back(
                new MenuCharacterSelect(CombatUI::getInstance()->mMonsterIndicator,
                CombatUI::getInstance()->mMonsterIndicatorPos,
                CombatUI::getInstance()->mMonsterList,
                new ListenerCoopMagic(),
                true));
            break;
        default:
            cout << "error mCurIconIndex " << mCurIconIndex << endl;
            break;
        };
    }
    else if (key == KEY_CANCEL)
    {
        CombatUI::getInstance()->onCancel();
    }
}

MenuCharacterSelect::MenuCharacterSelect(FrameAnimation *indicator, Point pos[3], vector<FightingCharacter*> list, OnCharacterSelectedListener *l, bool ignoreDead)
{
    mIndicator = indicator;

    mIndicatorPos.clear();
    mIndicatorPos.push_back(pos[0]);
    mIndicatorPos.push_back(pos[1]);
    mIndicatorPos.push_back(pos[2]);

    mList = list;
    mOnCharacterSelectedListener = l;
    mIgnoreDead = ignoreDead;
    for (int i = 0; i < list.size(); i++)
    {
        if (list.at(i)->isAlive())
        {
            mCurSel = i;
            break;
        }
    }
}

MenuCharacterSelect::~MenuCharacterSelect()
{

}

void MenuCharacterSelect::update(long delta)
{
    mIndicator->update(delta);
}

void MenuCharacterSelect::draw(Canvas *canvas)
{
    mIndicator->draw(canvas, mIndicatorPos.at(mCurSel).x, mIndicatorPos.at(mCurSel).y);
    if (mIndicator == CombatUI::getInstance()->mTargetIndicator)
    { // 当前选择角色
        Player *p = static_cast<Player*>(CombatUI::getInstance()->mPlayerList.at(mCurSel));
        CombatUI::getInstance()->mHeadsImg[p->getIndex() - 1]->draw(canvas, 1, 50, 63); // 角色头像
        if (p != NULL)
        {
            Util::drawSmallNum(canvas, p->getHP(), 79, 72); // hp
            Util::drawSmallNum(canvas, p->getMaxHP(), 104, 72); // maxhp
            Util::drawSmallNum(canvas, p->getMP(), 79, 83); // mp
            Util::drawSmallNum(canvas, p->getMaxMP(), 104, 83); // maxmp
        }
    }
}

void MenuCharacterSelect::selectNextTarget()
{
    do
    {
        ++mCurSel;
        mCurSel %= mList.size();
    } while (mIgnoreDead && !mList.at(mCurSel)->isAlive());
}

void MenuCharacterSelect::selectPreTarget()
{
    do
    {
        --mCurSel;
        mCurSel = (mCurSel + mList.size()) % mList.size();
    } while (mIgnoreDead && !mList.at(mCurSel)->isAlive());
}

void MenuCharacterSelect::onKeyDown(int key)
{
    if (key == KEY_RIGHT)
    {
        selectNextTarget();
    }
    else if (key == KEY_LEFT)
    {
        selectPreTarget();
    }
}

void MenuCharacterSelect::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        delete mOnCharacterSelectedListener;

        delete CombatUI::getInstance()->mScreenStack.back();
        CombatUI::getInstance()->mScreenStack.pop_back();
    }
    else if (key == KEY_ENTER)
    {
        OnCharacterSelectedListener *listener = mOnCharacterSelectedListener;
        vector<FightingCharacter*> list = mList;
        int curSel = mCurSel;

        delete CombatUI::getInstance()->mScreenStack.back();
        CombatUI::getInstance()->mScreenStack.pop_back();

        if (listener != NULL)
        {
            listener->onCharacterSelected(list.at(curSel));
            delete listener;
        }
    }
}

MenuMisc::MenuMisc()
{
    mBg = Util::getFrameBitmap(2 * 16 + 6, 5 * 16 + 6);

    mText = "围攻道具防御逃跑状态";

    mItemText.clear();
    mItemText.push_back("围攻");
    mItemText.push_back("道具");
    mItemText.push_back("防御");
    mItemText.push_back("逃跑");
    mItemText.push_back("状态");

    mTextRect = new Rect(9 + 3, 4 + 3, 9 + 4 + 16 * 2, 4 + 3 + 16 * 5);

    mCurSelIndex = 0;
}

MenuMisc::~MenuMisc()
{
    delete mBg;
    delete mTextRect;
}

void MenuMisc::draw(Canvas *canvas)
{
    canvas->drawBitmap(mBg, 9, 4);
    TextRender::drawText(canvas, mText, 0, mTextRect);
    TextRender::drawSelText(canvas, mItemText.at(mCurSelIndex), mTextRect->left, mTextRect->top + mCurSelIndex * 16);
}

void MenuMisc::onKeyDown(int key)
{
    if (key == KEY_UP)
    {
        --mCurSelIndex;
        mCurSelIndex = (mItemText.size() + mCurSelIndex) % mItemText.size();
    }
    else if (key == KEY_DOWN)
    {
        ++mCurSelIndex;
        mCurSelIndex %= mItemText.size();
    }
}

void MenuMisc::onKeyUp(int key)
{
    if (key == KEY_ENTER)
    {
        switch (mCurSelIndex)
        {
        case 0:// 围攻
            if (CombatUI::getInstance()->mCallBack != NULL)
            {
                CombatUI::getInstance()->mCallBack->onAutoAttack();
            }
            break;
        case 1:// 道具
            CombatUI::getInstance()->mScreenStack.push_back(new MenuGoods());
            break;
        case 2:// 防御
        {
            Player *p = CombatUI::getInstance()->getCurPlayer();
            p->getFightingSprite()->setCurrentFrame(9);
            CombatUI::getInstance()->onActionSelected(new ActionDefend(p));
            break;
        }   
        case 3:// 逃跑
            if (CombatUI::getInstance()->mCallBack != NULL)
            {
                CombatUI::getInstance()->mCallBack->onFlee();
            }
            break;
        case 4://状态
             delete CombatUI::getInstance()->mScreenStack.back();
             CombatUI::getInstance()->mScreenStack.pop_back();
             CombatUI::getInstance()->mScreenStack.push_back(new MenuState());
            break;
        }
    }
    else if (key == KEY_CANCEL)
    {
        delete CombatUI::getInstance()->mScreenStack.back();
        CombatUI::getInstance()->mScreenStack.pop_back();
    }
}

MenuState::MenuState()
{
    mBg = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 2, 11);
    mMarker = (ResImage *)DatLib::GetRes(DatLib::RES_PIC, 2, 12);

    mCurPlayer = CombatUI::getInstance()->mCurPlayerIndex;
}

MenuState::~MenuState()
{
    delete mBg;
    delete mMarker;
}

void MenuState::draw(Canvas *canvas)
{
    int x = (160 - mBg->getWidth()) / 2;
    int y = (96 - mBg->getHeight()) / 2;
    mBg->draw(canvas, 1, x, y);
    Player *p = static_cast<Player*>(CombatUI::getInstance()->mPlayerList.at(this->mCurPlayer));
    p->drawHead(canvas, x + 7, y + 4);
    Util::drawSmallNum(canvas, p->getHP(), x + 50, y + 9); // 命
    Util::drawSmallNum(canvas, p->getAttack(), x + 50, y + 21); // 攻
    Util::drawSmallNum(canvas, p->getLuck(), x + 87, y + 9); // 运
    Util::drawSmallNum(canvas, p->getSpeed(), x + 87, y + 21); // 身
    // TODO 更多的效果
    mMarker->draw(canvas, 1, x + 9, y + 48); // 攻
    mMarker->draw(canvas, 2, x + 25, y + 48); // 防
    mMarker->draw(canvas, 5, x + 41, y + 48); // 身
    mMarker->draw(canvas, 3, x + 57, y + 48); // 毒
    mMarker->draw(canvas, 4, x + 73, y + 48); // 乱
    mMarker->draw(canvas, 3, x + 88, y + 48); // 封
    mMarker->draw(canvas, 4, x + 104, y + 48); // 眠
    Util::drawSmallNum(canvas, 5, x + 10, y + 57); // 攻
    Util::drawSmallNum(canvas, 5, x + 26, y + 57); // 防
    Util::drawSmallNum(canvas, 5, x + 42, y + 57); // 身
    Util::drawSmallNum(canvas, 5, x + 58, y + 57); // 毒
    Util::drawSmallNum(canvas, 5, x + 74, y + 57); // 乱
    Util::drawSmallNum(canvas, 5, x + 90, y + 57); // 封
    Util::drawSmallNum(canvas, 5, x + 106, y + 57); // 眠
}

void MenuState::onKeyDown(int key)
{
    switch (key)
    {
    case KEY_RIGHT:
    case KEY_DOWN:
    case KEY_PAGEDOWN:
    case KEY_ENTER:
        ++this->mCurPlayer;
        this->mCurPlayer %= CombatUI::getInstance()->mPlayerList.size();
        break;

    case KEY_LEFT:
    case KEY_UP:
    case KEY_PAGEUP:
        --this->mCurPlayer;
        this->mCurPlayer = (this->mCurPlayer + CombatUI::getInstance()->mPlayerList.size()) % CombatUI::getInstance()->mPlayerList.size();
        break;
    }
}

void MenuState::onKeyUp(int key)
{
    if (key == KEY_CANCEL)
    {
        delete CombatUI::getInstance()->mScreenStack.back();
        CombatUI::getInstance()->mScreenStack.pop_back();

        CombatUI::getInstance()->mScreenStack.push_back(new MenuMisc());
    }
}

MenuGoods::MenuGoods()
{
    mBg = Util::getFrameBitmap(16 * 2 + 6, 16 * 3 + 6);

    mText = "装备投掷使用";

    mItemText.clear();
    mItemText.push_back("装备");
    mItemText.push_back("投掷");
    mItemText.push_back("使用");

    mTextRect = new Rect(29 + 3, 14 + 3, 29 + 3 + mBg->getWidth(), 14 + 3 + mBg->getHeight());
    
    mSelIndex = 0;
}

MenuGoods::~MenuGoods()
{
    delete mBg;
    delete mTextRect;
}

void MenuGoods::draw(Canvas *canvas)
{
    canvas->drawBitmap(mBg, 29, 14);
    TextRender::drawText(canvas, mText, 0, mTextRect);
    TextRender::drawSelText(canvas, mItemText.at(mSelIndex), mTextRect->left, mTextRect->top + 16 * mSelIndex);
}

void MenuGoods::onKeyDown(int key)
{
    if (key == KEY_DOWN)
    {
        ++mSelIndex;
        mSelIndex %= mItemText.size();
    }
    else if (key == KEY_UP)
    {
        --mSelIndex;
        mSelIndex = (mSelIndex + mItemText.size()) % mItemText.size();
    }
}

void MenuGoods::onKeyUp(int key)
{
    int selIndex = mSelIndex;

    if (key == KEY_ENTER)
    {
        // 弹出子菜单
        delete CombatUI::getInstance()->mScreenStack.back();
        CombatUI::getInstance()->mScreenStack.pop_back();

        switch (selIndex)
        {
        case 0:// 装备
            struct EquipListListener : public ScreenGoodsList::OnItemSelectedListener
            {
                virtual void onItemSelected(BaseGoods *goods)
                {
                    equipSelected(goods);
                }
            };

            GameView::getInstance()->pushScreen(new ScreenGoodsList(
                Player::sGoodsList->getEquipList(),
                new EquipListListener(),
                ScreenGoodsList::Use));
            break;

        case 1:// 投掷
            struct ThrowableGoodsListListener : public ScreenGoodsList::OnItemSelectedListener
            {
                virtual void onItemSelected(BaseGoods *goods)
                {
                    GameView::getInstance()->popScreen(); // pop goods list

                    delete CombatUI::getInstance()->mScreenStack.back();
                    CombatUI::getInstance()->mScreenStack.pop_back();// pop misc menu

                    if (goods->effectAll())
                    {
                         // 投掷伤害全体敌人
                        CombatUI::getInstance()->onActionSelected(new ActionThrowItemAll(
                            CombatUI::getInstance()->getCurPlayer(),
                            CombatUI::getInstance()->mMonsterList,
                            static_cast<GoodsHiddenWeapon *>(goods)));
                    }
                    else
                    { 
                        struct ListenerSelectedOneMonster : public OnCharacterSelectedListener
                        {
                            BaseGoods *goods;
                            ListenerSelectedOneMonster(BaseGoods *g)
                                :goods(g)
                            {
                            }
                            virtual void onCharacterSelected(FightingCharacter *fc)
                            {
                                // add throw action
                                CombatUI::getInstance()->onActionSelected(new ActionThrowItemOne(
                                    CombatUI::getInstance()->getCurPlayer(),
                                    fc,
                                    static_cast<GoodsHiddenWeapon *>(goods)));
                            }
                        };

                        // 选一个敌人
                        CombatUI::getInstance()->mScreenStack.push(new MenuCharacterSelect(
                            CombatUI::getInstance()->mMonsterIndicator, 
                            CombatUI::getInstance()->mMonsterIndicatorPos, 
                            CombatUI::getInstance()->mMonsterList,
                            new ListenerSelectedOneMonster(goods),
                            true));
                    }
                }
            };

            GameView::getInstance()->pushScreen(new ScreenGoodsList(
                getThrowableGoodsList(),
                new ThrowableGoodsListListener(),
                ScreenGoodsList::Use));
            break;
        case 2:// 使用
            struct UseableGoodsListListener : public ScreenGoodsList::OnItemSelectedListener
            {
                virtual void onItemSelected(BaseGoods *goods)
                {
                    GameView::getInstance()->popScreen(); // pop goods list

                    delete CombatUI::getInstance()->mScreenStack.back();
                    CombatUI::getInstance()->mScreenStack.pop_back();// pop misc menu

                    if (goods->effectAll())
                    {
                        CombatUI::getInstance()->onActionSelected(new ActionUseItemAll(
                            CombatUI::getInstance()->getCurPlayer(),
                            CombatUI::getInstance()->mMonsterList,
                            goods));
                    }
                    else
                    { 
                        struct ListenerUseItemOne : public OnCharacterSelectedListener
                        {
                            BaseGoods *goods;
                            ListenerUseItemOne(BaseGoods *g)
                                :goods(g)
                            {
                            }
                            virtual void onCharacterSelected(FightingCharacter *fc)
                            {
                                CombatUI::getInstance()->onActionSelected(new ActionUseItemOne(
                                    CombatUI::getInstance()->getCurPlayer(),
                                    fc,
                                    goods));
                            }
                        };

                        // 选一个角色治疗
                        CombatUI::getInstance()->mScreenStack.push(new MenuCharacterSelect(
                            CombatUI::getInstance()->mTargetIndicator, 
                            CombatUI::getInstance()->mPlayerIndicatorPos, 
                            CombatUI::getInstance()->mPlayerList,
                            new ListenerUseItemOne(goods),
                            false));
                    }
                }
            };

            GameView::getInstance()->pushScreen(new ScreenGoodsList(
                getUseableGoodsList(),
                new UseableGoodsListListener(),
                ScreenGoodsList::Use));
            break;
        }
    }
    else if (key == KEY_CANCEL)
    {
        delete CombatUI::getInstance()->mScreenStack.back();
        CombatUI::getInstance()->mScreenStack.pop_back();
    }
}

std::vector<BaseGoods *> *MenuGoods::getUseableGoodsList()
{
    static vector<BaseGoods*> *rlt = NULL;
    if (rlt == NULL)
    {
        rlt = new vector < BaseGoods * > ;
    }
    rlt->clear();
    vector<BaseGoods *>  *goodsList = Player::sGoodsList->getGoodsList();

    for (vector<BaseGoods*>::const_iterator iter = goodsList->begin(); iter != goodsList->end(); ++iter)
    {
        BaseGoods* g = *iter;
        switch (g->getType())
        {
        case 9:
        case 10:
        case 11:
        case 12:
            rlt->push_back(g);
            break;
        }
    }

    return rlt;
}

std::vector<BaseGoods *> *MenuGoods::getThrowableGoodsList()
{
    static vector<BaseGoods*> *rlt = NULL;
    if (rlt == NULL)
    {
        rlt = new vector < BaseGoods * >;
    }
    rlt->clear();

    vector<BaseGoods *> *goodsList = Player::sGoodsList->getGoodsList();


    for (vector<BaseGoods*>::const_iterator iter = goodsList->begin(); iter != goodsList->end(); ++iter)
    {
        BaseGoods* g = *iter;
        if (g->getType() == 8)
        {
            rlt->push_back(g);
        }
    }
    return rlt;
}

void MenuGoods::equipSelected(BaseGoods *goods)
{
    vector<Player*> list;
    list.clear();

    for (int i = 0; i < CombatUI::getInstance()->mPlayerList.size(); i++)
    {
        Player *p = static_cast<Player *>(CombatUI::getInstance()->mPlayerList.at(i));
        if (goods->canPlayerUse(p->getIndex()))
        {
            list.push_back(p);
        }
    }

    if (list.size() == 0)
    { // 没人能装备
        Util::showMessage("不能装备!", 1000);
    }
    else if (list.size() == 1)
    { // 一个人能装备
        if (list.at(0)->hasEquipt(goods->getType(), goods->getIndex()))
        {
            Util::showMessage("已装备!", 1000);
        }
        else
        {
            GameView::getInstance()->pushScreen(new ScreenChgEquipment(list.at(0), (GoodsEquipment *)goods));
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
        };

        GameView::getInstance()->pushScreen(new MutilPlayerEquip(list, goods));
    }
}


