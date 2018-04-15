#include "GoodsManage.h"
#include "SaveLoadStream.h"

BaseGoods * GoodsManage::getEquip(int i)
{
    if (i >= 0 && i < (int)(mEquipList.size()))
    {
        return mEquipList[i];
    }
    return NULL;
}

int GoodsManage::getEquitTypeNum()
{
    return mEquipList.size();
}

BaseGoods * GoodsManage::getGoods(int i)
{
    if (i >= 0 && i < (int)(mGoodsList.size()))
    {
        return mGoodsList[i];
    }
    return NULL;
}

BaseGoods * GoodsManage::getGoods(int type, int index)
{
    if (type >= 1 && type <= 7)
    {
        return getGoods(mEquipList, type, index);
    }
    else if (type >= 8 && type <= 14)
    {
        return getGoods(mGoodsList, type, index);
    }
    return NULL;
}

BaseGoods * GoodsManage::getGoods(std::vector<BaseGoods *> &list, int type, int index)
{
    std::vector<BaseGoods *>::const_iterator iter;
    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        BaseGoods *i = *iter;
        if (i->getType() == type && i->getIndex() == index)
        {
            return i;
        }
    }
    return NULL;
}

int GoodsManage::getGoodsNum(int type, int index)
{
    int num = 0;
    if (type >= 1 && type <= 7)
    {
        num = getGoodsNum(mEquipList, type, index);
    }
    else if (type >= 8 && type <= 14)
    {
        num = getGoodsNum(mGoodsList, type, index);
    }
    return num;
}

int GoodsManage::getGoodsNum(std::vector<BaseGoods *> &list, int type, int index)
{
    std::vector<BaseGoods *>::const_iterator iter;
    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        BaseGoods *i = *iter;
        if (i->getType() == type && i->getIndex() == index)
        {
            return i->getGoodsNum();;
        }
    }
    return 0;
}

void GoodsManage::addGoods(int type, int index, int num)
{
    if (type >= 1 && type <= 7)
    {
        addGoods(mEquipList, type, index, num);
    }
    else if (type >= 8 && type <= 14)
    {
        addGoods(mGoodsList, type, index, num);
    }
}

void GoodsManage::addGoods(int type, int index)
{
    if (type >= 1 && type <= 7)   // 装备
    {
        addGoods(mEquipList, type, index, 1);
    }
    else if (type >= 8 && type <= 14)     // 物品，使用一次
    {
        addGoods(mGoodsList, type, index, 1);
    }
}

void GoodsManage::addGoods(std::vector<BaseGoods *> &list, int type, int index, int num)
{
    std::vector<BaseGoods *>::const_iterator iter;
    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        BaseGoods *i = *iter;
        if (i->getType() == type && i->getIndex() == index)
        {
            // 数量+1
            i->addGoodsNum(num);
            return;
        }
    }
    // 加入链表
    BaseGoods *item = (BaseGoods *)DatLib::GetRes(DatLib::RES_GRS, type, index);
    item->setGoodsNum(num);
    list.push_back(item);
}

bool GoodsManage::deleteGoods(int type, int index)
{
    return useGoodsNum(type, index, 1);
}

bool GoodsManage::deleteGoods(std::vector<BaseGoods *> &list, int type, int index, int num)
{
    std::vector<BaseGoods *>::const_iterator iter;
    for (iter = list.begin(); iter != list.end(); ++iter)
    {
        BaseGoods *i = *iter;
        if (i->getType() == type && i->getIndex() == index)
        {
            // 数量-num
            if (i->getGoodsNum() < num)
            {
                return false;
            }
            i->addGoodsNum(-num);
            if (i->getGoodsNum() <= 0)
            {
                list.erase(iter);
                delete i;
            }
            return true;
        }
    }
    return false;
}

void GoodsManage::clear()
{
    while (!mEquipList.empty())
    {
        delete mEquipList.back();
        mEquipList.pop_back();
    }

    while (!mGoodsList.empty())
    {
        delete mGoodsList.back();
        mGoodsList.pop_back();
    }
}

void GoodsManage::read(SaveLoadStream *buff)
{
    this->clear();
    int size = buff->readInt();
    for (int i = 0; i < size; i++)
    {
        int type = buff->readInt();
        int index = buff->readInt();
        BaseGoods *g = (BaseGoods*)DatLib::GetRes(DatLib::RES_GRS, type, index);
        g->setGoodsNum(buff->readInt());
        mEquipList.push_back(g);
    }

    size = buff->readInt();
    for (int i = 0; i < size; i++)
    {
        int type = buff->readInt();
        int index = buff->readInt();
        BaseGoods *g = (BaseGoods*)DatLib::GetRes(DatLib::RES_GRS, type, index);
        g->setGoodsNum(buff->readInt());
        mGoodsList.push_back(g);
    }
}

void GoodsManage::write(SaveLoadStream *buff)
{
    int size = mEquipList.size();
    buff->writeInt(size);
    for (int i = 0; i < size; i++)
    {
        BaseGoods *g = mEquipList.at(i);
        buff->writeInt(g->getType());
        buff->writeInt(g->getIndex());
        buff->writeInt(g->getGoodsNum());
    }

    size = mGoodsList.size();
    buff->writeInt(size);
    for (int i = 0; i < size; i++)
    {
        BaseGoods *g = mGoodsList.at(i);
        buff->writeInt(g->getType());
        buff->writeInt(g->getIndex());
        buff->writeInt(g->getGoodsNum());
    }
}

bool GoodsManage::useGoodsNum(int type, int index, int num)
{
    if (type >= 1 && type <= 7)   // 装备
    {
        return deleteGoods(mEquipList, type, index, num);
    }
    else if (type >= 8 && type <= 14)     // 物品，使用一次
    {
        return deleteGoods(mGoodsList, type, index, num);
    }
    return false;
}

int GoodsManage::getGoodsTypeNum()
{
    return mGoodsList.size();
}
