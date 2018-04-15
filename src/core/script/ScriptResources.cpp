#include "ScriptResources.h"
#include "SaveLoadStream.h"

ScriptResources::~ScriptResources()
{

}

void ScriptResources::initLocalVar()
{
    for (int i = 200; i < 240; i++)
    {
        variables[i] = 0;
    }
}

void ScriptResources::initGlobalVar()
{
    for (int i = 0; i < 200; i++)
    {
        variables[i] = 0;
    }
}

void ScriptResources::initGlobalEvents()
{
    for (int i = 1; i <= 2400; i++)
    {
        globalEvents[i] = false;
    }
}

void ScriptResources::setEvent(int num)
{
    globalEvents[num] = true;
}

void ScriptResources::clearEvent(int num)
{
    globalEvents[num] = false;
}

int ScriptResources::variables[240] = { 0 };
bool ScriptResources::globalEvents[2401] = {false};

bool ScriptResources::write(SaveLoadStream *Buffer)
{
   // 写全局事件
   for (int i = 1; i <= 2400; ++i)
   {
       Buffer->writeBoolean(globalEvents[i]);
   }

   // 写全局变量&局部变量
   for (int i = 0; i < 240; ++i)
   {
       Buffer->writeInt(variables[i]);
   }
    return true;
}

bool ScriptResources::read(SaveLoadStream *Buffer)
{
   // 读全局事件
   for (int i = 1; i <= 2400; ++i)
   {
       globalEvents[i] = Buffer->readBoolean();
   }

   // 读全局变量&局部变量
   for (int i = 0; i < 240; ++i)
   {
       variables[i] = Buffer->readInt();
   }
    return true;
}

