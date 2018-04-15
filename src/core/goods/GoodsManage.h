#ifndef _GoodsManage_H_
#define _GoodsManage_H_

#include <iostream>
#include "DatLib.h"
#include "BaseGoods.h"

class SaveLoadStream;

/**
 * 物品管理
 * @author Chen
 *
 */
class GoodsManage
{
private:
    /**
     * 保存只能用一次的物品，暗器、药品等
     */
    std::vector<BaseGoods *> mGoodsList;
    /**
     * 装备链表
     */
    std::vector<BaseGoods *> mEquipList;

public:
    std::vector<BaseGoods *> *getGoodsList(){return &mGoodsList;}
    std::vector<BaseGoods *> *getEquipList(){return &mEquipList;}

    /**
     * 获取装备链表中的i号装备
     * @param i
     * @return 当<code>i < 0 || i >= list.size()</code>时，返回<code>NULL</code>
     */
    BaseGoods *getEquip(int i);

    /**
     * 返回装备种类数目
     * @return
     */
    int getEquitTypeNum();

    /**
     * 获取一次性物品链表中i位置的物品
     * @param i
     * @return 当<code>i < 0 || i >= list.size()</code>时，返回<code>NULL</code>
     */
    BaseGoods *getGoods(int i);

    /**
     * 返回一次性物品种类数目
     * @return
     */
    int getGoodsTypeNum();

    /**
     * 获取链表中type index号物品
     * @param type
     * @param index
     * @return 当该种物品不存在时返回<code>NULL</code>
     */
    BaseGoods *getGoods(int type, int index);
private:
    BaseGoods *getGoods(std::vector<BaseGoods *> &list, int type, int index);
    /**
     * 得到type index号物品的数量
     * @param type
     * @param index
     * @return 物品数量
     */
public:
    int getGoodsNum(int type, int index);
private:
    int getGoodsNum(std::vector<BaseGoods *> &list, int type, int index);
public:
    void addGoods(int type, int index, int num);
    void addGoods(int type, int index);

private:
    /**
     * 将type index号物品添加到list链表中<p>
     * 如果有该号物品，则计数加一<p>
     * 没有则加入该物品
     * @param list
     * @param type
     * @param index
     */
    void addGoods(std::vector<BaseGoods *> &list, int type, int index, int num);

public:
    /**
     * 物品数量减一，如果数量为0，则删除物品
     * @param type
     * @param index
     * @return 删除成功返回<code>true</code> 不存在该物品返回<code>false</code>
     */
    bool deleteGoods(int type, int index);

    /**
     * 物品数量减num，如果数量不足则返回false
     * @param type
     * @param index
     * @return
     */
    bool useGoodsNum(int type, int index, int num);

private:
    bool deleteGoods(std::vector<BaseGoods *> &list, int type, int index, int num);

public:
    void clear();

    void read(SaveLoadStream *buff);
    void write(SaveLoadStream *buff);
 
};

#endif
