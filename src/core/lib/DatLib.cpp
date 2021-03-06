#include "DatLib.h"
#include "BaseGoods.h"
#include "GoodsEquipment.h"
#include "GoodsStimulant.h"
#include "GoodsTudun.h"
#include "GoodsWeapon.h"
#include "GoodsMedicineChg4Ever.h"
#include "GoodsHiddenWeapon.h"
#include "GoodsDrama.h"
#include "GoodsMedicine.h"
#include "GoodsMedicineLife.h"
#include "GoodsDecorations.h"
#include "NPC.h"
#include "SceneObj.h"
#include "MagicAttack.h"
#include "MagicAuxiliary.h"
#include "MagicEnhance.h"
#include "MagicSpecial.h"
#include "MagicRestore.h"
#include "ResLevelupChain.h"
#include "Monster.h"


DatLib *DatLib::Instance = NULL;

DatLib::DatLib()
{
    std::ifstream in;
    in.open(std::string("./dat/DAT.LIB"), std::ios::in | std::ios::binary);
    if (!in.is_open())
    {
        std::cout << "Error opening file";
        exit(1);
    }

    std::streampos pos = in.tellg();
    in.seekg(0, std::ios::end);
    long size = (long)in.tellg();
    in.seekg(pos);

    mBuffer = new char[size];
    memset(mBuffer, 0x00, size);

    mBufferSize = size;
    in.read(mBuffer, size);
    in.close();

    getAllResOffset();
}

DatLib::~DatLib()
{
    delete mBuffer;
}

void DatLib::getAllResOffset()
{
    int i = 0x10, j = 0x2000;
    mDataOffset.clear();

    while (mBuffer[i] != -1)
    {
        int resType = ((int)mBuffer[i++]) & 0xFF;
        int type = ((int)mBuffer[i++]) & 0xFF;
        int index = ((int)mBuffer[i++]) & 0xFF;
        int key = getKey(resType, type, index);
        int block = ((int)mBuffer[j++]) & 0xFF;
        int low = ((int)mBuffer[j++]) & 0xFF;
        int high = ((int)mBuffer[j++]) & 0xFF;
        int value = block * 0x4000 | (high << 8 | low);
        mDataOffset.insert(Int_Pair(key, value));
    }
}



ResBase *DatLib::GetRes(int resType, int type, int index)
{
    return Instance->getRes(resType, type, index);
}

ResBase *DatLib::getRes(int resType, int type, int index)
{
    ResBase *rtn = NULL;
    int offset = getDataOffset(resType, type, index);

    if (offset != -1)
    {
        switch (resType)
        {
        case RES_GUT:
            rtn = new ResGut();
            break;

        case RES_MAP:
            rtn = new ResMap();
            break;

        case RES_ARS:
            switch (type)
            {
            case 1: // 玩家角色
                rtn = new Player();
                break;

            case 2: // NPC角色
                rtn = new NPC();
                break;

            case 3: // 敌人角色
                rtn = new Monster();
                break;

            case 4: // 场景对象
                rtn = new SceneObj();
                break;

            default:
                rtn = NULL;
                break;
            }
            break;

        case RES_MRS:
            rtn = getMagic(type, index);
            break;

        case RES_SRS:
            rtn = new ResSrs();
            break;

        case RES_GRS:
            rtn = getGoods(type, index);
            break;

        case RES_TIL:
        case RES_ACP:
        case RES_GDP:
        case RES_GGJ:
        case RES_PIC:
            rtn = new ResImage();
            break;

        case RES_MLR:
            if (type == 1)
            {
                rtn = new ResMagicChain();
            }
            else if (type == 2)
            {
                rtn = new ResLevelupChain();
            }
            break;
        }

        rtn->setData(mBuffer, offset);
    }
    else
    {
        // 资源不存在
        std::cout << "DatLib.getRes" << "resType:" << resType <<
            " type:" << type << " index:" << index << " not found." << std::endl;
    }

    return rtn;
}

BaseGoods *DatLib::getGoods(int type, int index)
{
    if (type >= 1 && type <= 5)
    {
        return new GoodsEquipment();
    }
    BaseGoods *rtn = NULL;
    switch (type)
    {
    case 6:
        rtn = new GoodsDecorations();
        break;

    case 7:
        rtn = new GoodsWeapon();
        break;

    case 8:
        rtn = new GoodsHiddenWeapon();
        break;

    case 9:
        rtn = new GoodsMedicine();
        break;

    case 10:
        rtn = new GoodsMedicineLife();
        break;

    case 11:
        rtn = new GoodsMedicineChg4Ever();
        break;

    case 12:
        rtn = new GoodsStimulant();
        break;

    case 13:
        rtn = new GoodsTudun();
        break;

    case 14:
        rtn = new GoodsDrama();
        break;
    }
    return rtn;
}

ResBase *DatLib::getMagic(int type, int index)
{
    switch (type)
    {
    case 1:
        return new MagicAttack();
    case 2:
        return new MagicEnhance();
    case 3:
        return new MagicRestore();
    case 4:
        return new MagicAuxiliary();
    case 5:
        return new MagicSpecial();
    default:
        break;
    }
    return NULL;
}

int DatLib::getDataOffset(int resType, int type, int index)
{
    int key = getKey(resType, type, index);
    std::hash_map <int, int> ::const_iterator iter = mDataOffset.find(key);
    int i = iter->second;
    if (i == NULL)
    {
        return -1;
    }
    return i;
}

int DatLib::getKey(int resType, int type, int index)
{
    return (resType << 16) | (type << 8) | index;
}

void DatLib::InitDatLib()
{
    Instance = new DatLib();
}

DatLib * DatLib::getInstance()
{
    if (NULL == Instance)
    {
        Instance = new DatLib();
    }

    return Instance;
}

