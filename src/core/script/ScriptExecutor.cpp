#include "ScriptExecutor.h"

ScriptExecutor::ScriptExecutor(std::vector<Operate *> &list, std::vector<int> &eventIndex, std::hash_map<int, int> &map, int headerCnt)
{
    mOperateList = list;
    mEventIndex = eventIndex;
    mCurExeOperateIndex = 0;
    mIsExeUpdateDraw = false;
    mMapAddrOffsetIndex = map;
    mHeaderCnt = headerCnt;
}

ScriptExecutor::~ScriptExecutor()
{
    for (unsigned int i = 0; i < mOperateList.size(); i++)
    {
        if (NULL != mOperateList[i])
        {
            delete mOperateList[i];
        }
    }
}

bool ScriptExecutor::triggerEvent(int eventId)
{
    if (eventId > (int)(mEventIndex.size()))
    {
        return false;
    }

    int index = mEventIndex[eventId - 1];
    if (index != -1)
    {
        mCurExeOperateIndex = index;
        mIsExeUpdateDraw = false;
        return true;
    }
    return false;
}

void ScriptExecutor::gotoAddress(int address)
{
    std::hash_map <int, int> ::const_iterator iter = mMapAddrOffsetIndex.find(address - mHeaderCnt);
    mCurExeOperateIndex = iter->second;;

    if (mIsExeUpdateDraw)   // 不在Operate.process()中调用的gotoAddress
    {
        mIsExeUpdateDraw = false;
        --mCurExeOperateIndex;
    }
    else     // 在Operate.process()中调用的gotoAddress
    {
        goonExecute = false; // mark 下次调用process再执行
    }
}

void ScriptExecutor::process()
{
    if (!mIsExeUpdateDraw)
    {
        for (; mCurExeOperateIndex < (int)(mOperateList.size()) && goonExecute;)
        {
            Operate *oper = mOperateList.at(mCurExeOperateIndex);
            if (oper != NULL && oper->process())   // 执行 update draw
            {
                mIsExeUpdateDraw = true;
                return;
            }
            if (!goonExecute)
            {
                goonExecute = true;
                return;
            }
            ++mCurExeOperateIndex;
        }
        // 正常情况不回执行到这里，脚本最后一句是callback
    }
}

void ScriptExecutor::update(long delta)
{
    if (mIsExeUpdateDraw)
    {
        if (!mOperateList.at(mCurExeOperateIndex)->update(delta))   // 退出当前操作
        {
            mIsExeUpdateDraw = false;
            ++mCurExeOperateIndex;
        }
    }
}

void ScriptExecutor::draw(Canvas *canvas)
{
    if (mIsExeUpdateDraw)
    {
        mOperateList.at(mCurExeOperateIndex)->draw(canvas);
    }
}

void ScriptExecutor::keyDown(int key)
{
    if (mIsExeUpdateDraw)
    {
        mOperateList.at(mCurExeOperateIndex)->onKeyDown(key);
    }
}

void ScriptExecutor::keyUp(int key)
{
    if (mIsExeUpdateDraw)
    {
        mOperateList.at(mCurExeOperateIndex)->onKeyUp(key);
    }
}

bool ScriptExecutor::goonExecute = true;
