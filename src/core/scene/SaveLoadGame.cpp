#include "SaveLoadGame.h"
#include "SaveLoadStream.h"
#include "ScreenMainGame.h"
#include "Combat.h"
#include "SceneObj.h"
#include "NPC.h"
#include "Player.h"

SaveLoadGame::SaveLoadGame()
{

}

SaveLoadGame::~SaveLoadGame()
{

}

bool SaveLoadGame::write(SaveLoadStream *buff)
{
    buff->writeString(SceneName);
    int actorNum = ScreenMainGame::sPlayerList.size();
    buff->writeInt(actorNum);
    for (int i = 0; i < actorNum; i++)
    {
        buff->writeInt(ScreenMainGame::sPlayerList.at(i)->getIndex());
    }
    buff->writeInt(MapType);
    buff->writeInt(MapIndex);
    buff->writeInt(MapScreenX);
    buff->writeInt(MapScreenY);
    buff->writeInt(ScriptType);
    buff->writeInt(ScriptIndex);

    buff->writeInt(actorNum);
    for (int i = 0; i < actorNum; i++)
    {
        ScreenMainGame::sPlayerList.at(i)->write(buff);
    }

    buff->writeInt(Player::sMoney);

    Player::sGoodsList->write(buff);

    writeNpcObjs(buff);

    Combat::getInstance()->write(buff);
    return true;
}

bool SaveLoadGame::read(SaveLoadStream *buff)
{
    SceneName = buff->readString();

    int actorNum = buff->readInt();
    while (actorNum-- > 0)
    {
        int index = buff->readInt();
    }

    MapType = buff->readInt();
    MapIndex = buff->readInt();
    MapScreenX = buff->readInt();
    MapScreenY = buff->readInt();
    ScriptType = buff->readInt();
    ScriptIndex = buff->readInt();

    int size = buff->readInt();  
    while (!ScreenMainGame::sPlayerList.empty())
    {
        delete ScreenMainGame::sPlayerList.back();
        ScreenMainGame::sPlayerList.pop_back();
    }
    for (int i = 0; i < size; i++)
    {
        Player *p = new Player();
        p->read(buff);
        ScreenMainGame::sPlayerList.push_back(p);
    }
    Player::sMoney = buff->readInt();
    Player::sGoodsList->read(buff);

    readNpcObjs(buff);

    Combat::getInstance()->read(buff);
    return true;
}


void SaveLoadGame::readNpcObjs(SaveLoadStream *buff)
{
    int sum = buff->readInt();
    for (int i = 0; i < 41; i++)
    {
        if (NULL != ScreenMainGame::sNPCObjs[i])
        {
            delete ScreenMainGame::sNPCObjs[i];
            ScreenMainGame::sNPCObjs[i] = NULL;
        }
    }
    for (int i = 0; i < sum; i++)
    {
        int index = buff->readInt();
        bool sceneObj = buff->readBoolean();
        if (sceneObj)
        {
            ScreenMainGame::sNPCObjs[index] = new SceneObj();
        }
        else
        {
            ScreenMainGame::sNPCObjs[index] = new NPC();
        }
    }
    for (int i = 0; i < 41; i++)
    {
        if (NULL != ScreenMainGame::sNPCObjs[i])
        {
            NPC *npc = ScreenMainGame::sNPCObjs[i];
            if (npc->bInstanceOf_SceneObj)
            {
                static_cast<SceneObj *>(npc)->read(buff);
            }
            else
            {
                npc->read(buff);
            }
        }
    }
}

void SaveLoadGame::writeNpcObjs(SaveLoadStream *buff)
{
    int sum = 0;
    for (int i = 0; i < 41; i++)
    {
        NPC *npc = ScreenMainGame::sNPCObjs[i];
        if (NULL != npc)
        {
            sum++;
        }
    }
    buff->writeInt(sum);
    for (int i = 0; i < 41; i++)
    {
        NPC *npc = ScreenMainGame::sNPCObjs[i];
        if (NULL != npc)
        {
            buff->writeInt(i);
            buff->writeBoolean(npc->bInstanceOf_SceneObj);
        }
    }
    for (int i = 0; i < 41; i++)
    {
        if (NULL != ScreenMainGame::sNPCObjs[i])
        {
            NPC *npc = ScreenMainGame::sNPCObjs[i];
            if (npc->bInstanceOf_SceneObj)
            {
                static_cast<SceneObj *>(npc)->write(buff);
            }
            else
            {
                npc->write(buff);
            }
        }
    }
}

std::string SaveLoadGame::SceneName;
int SaveLoadGame::ScriptIndex = 1;
int SaveLoadGame::ScriptType = 1;
int SaveLoadGame::MapScreenY = 1;
int SaveLoadGame::MapScreenX = 1;
int SaveLoadGame::MapIndex = 1;
int SaveLoadGame::MapType = 1;
bool SaveLoadGame::startNewGame = true;