#ifndef _BaseGoods_H_
#define _BaseGoods_H_

#include <iostream>
#include "DatLib.h"
#include "ResBase.h"
#include "ResImage.h"
#include "Canvas.h"

class BaseGoods: public ResBase
{
public:
    BaseGoods();
    virtual ~BaseGoods();

    virtual void setData(char * buf, int offset);
    /**
     *
     * @param playId 1-4
     * @return
     */
    bool canPlayerUse(int playId);
    int getSumRound();
    void draw(Canvas *canvas, int x, int y);
    std::string getName();
    int getBuyPrice();
    int getSellPrice();
    std::string getDescription();
    int getEventId();
    int getGoodsNum();
    void setGoodsNum(int num);
    
    /**
     * 增加物品数量
     * @param d 增量
     */
    void addGoodsNum(int d);
    
    /** 是否具有全体效果*/
    bool effectAll();

    ///**
    // * 比较物品编号是否相等
    // */
    //virtual bool equals(Object o)
    //{
    //    return mType == ((BaseGoods)o).getType() && mIndex == ((BaseGoods)o).getIndex();
    //}

    int getAniType()const { return mAniType; }
    int getAniIndex()const { return mAniIndex; }

protected:
    virtual void setOtherData(char *buf, int offset) = 0;
private:
    /**
    * 是否可装备，最低位为主角1
    */
    int mEnable;
    int mSumRound; // 持续回合
    ResImage *mImage; // 物品图片
    std::string mName; // 道具名称
    int mBuyPrice; // 买价
    int mSellPrice;//卖价
    /**
     * 不为0时装备该道具时，就会设置该事件，而卸下时，
     * 就会取消该事件，不能用来典当。
     */
    int mEventId;
    /**
     * 物品数量
     */
    int mGoodsNum;
    std::string mDescription; // 道具说明
public:
    //动画素材编号
    int mAniType;
    int mAniIndex;

    bool bInstanceof_GoodsMedicine;
};

#endif
