#pragma once

#include "ScreenGoodsList.h"
#include "Operate.h"
#include "BaseScreen.h"

using namespace std;

class BaseGoods;

class OperateBuy : public Operate, public ScreenGoodsList::OnItemSelectedListener
{
private:
    const char *data;
    const int start;
    static vector<BaseGoods *> *goodsList;

public:
    OperateBuy(const char *data, int start);
    virtual bool process();
    virtual bool update(long delta){return false;}
    virtual void draw(Canvas *canvas);
    virtual void onKeyDown(int key){}
    virtual void onKeyUp(int key){}
    virtual void onItemSelected(BaseGoods *goods);

private:
    class BuyGoodsScreen : public BaseScreen
    {
    private:
        static Bitmap *bmpBg;
        BaseGoods *goods;
        int buyCnt;
        int money;
    public:
        BuyGoodsScreen(BaseGoods *goods);
        ~BuyGoodsScreen();
        void init(BaseGoods *goods);
        virtual void update(long delta){}
        virtual void draw(Canvas *canvas);
        virtual void onKeyUp(int key);
        virtual void onKeyDown(int key);
        virtual bool isPopup(){return true;}
    };
};
