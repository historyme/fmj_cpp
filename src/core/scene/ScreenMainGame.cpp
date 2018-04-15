#include "ScreenMainGame.h"
#include "SceneObj.h"
#include "ScreenGameMainMenu.h"
#include "Combat.h"

ScreenMainGame::ScreenMainGame()
{
    bInstanceof_ScreenMainGame = true;

    mNpcCanWalk = new NpcCanWalk();
    mSceneName = "";

    mRunScript = true;
    mScriptSys = ScriptProcess::getInstance();
    mScriptSys->setScreenMainGame(this);

    mCombat = Combat::getInstance();

    if (SaveLoadGame::startNewGame)   // 开始新游戏
    {
        for (int i = 0; i < 41; i++)
        {
            sNPCObjs[i] = NULL;
        }

        mCombat->fightDisable();
        ScriptResources::initGlobalVar();
        ScriptResources::initGlobalEvents();
        sPlayerList.clear();//TODO 内存释放
        Player::sGoodsList->clear(); //TODO 内存释放
        Player::sMoney = 0;
        startChapter(1, 1);
        ScriptExecutor::goonExecute = true;
        mRunScript = true;
    }
    else     // 再续前缘
    {
        loadMap(SaveLoadGame::MapType, SaveLoadGame::MapIndex,
                SaveLoadGame::MapScreenX, SaveLoadGame::MapScreenY);
        for (int i = 0; i < 41; i++)
        {
            if (sNPCObjs[i] != NULL)
            {
                sNPCObjs[i]->setICanWalk(mNpcCanWalk);
            }
        }
        if (sPlayerList.size() > 0)
        {
            sPlayer = sPlayerList[0];
        }
        else
        {
            createActor(1, 4, 3);
            //Log.e("error", "存档读取出错");
        }

        //切换场景需要删除以前的数据
        mScriptSys->initScript();
        if (NULL != mScriptExecutor)
        {
            delete mScriptExecutor;
        }

        mScriptSys->loadScript(SaveLoadGame::ScriptType, SaveLoadGame::ScriptIndex);
        mScriptExecutor = mScriptSys->getScriptExecutor();
        ScriptExecutor::goonExecute = true;
        mRunScript = false;
    }
}

void ScreenMainGame::setSceneName(std::string name)
{
    mSceneName = name;
    SaveLoadGame::SceneName = name;
}

void ScreenMainGame::exitScript()
{
    mRunScript = false;
    ScriptExecutor::goonExecute = false;
}


void ScreenMainGame::runScript()
{
    mRunScript = true;
}


void ScreenMainGame::startChapter(int type, int index)
{
    std::cout << "ScreenMainGame.startChapter " << type << " " << index << std::endl;

    //切换场景需要删除以前的数据
    mScriptSys->initScript();
    if (NULL != mScriptExecutor)
    {
        delete mScriptExecutor;
    }

    mScriptSys->loadScript(type, index);
    mScriptExecutor = mScriptSys->getScriptExecutor();

    ScriptExecutor::goonExecute = false;
    for (int i = 1; i <= 40; i++)
    {
        if (NULL != sNPCObjs[i])
        {
            delete sNPCObjs[i];
            sNPCObjs[i] = NULL;
        }

    }
    ScriptResources::initLocalVar();
    SaveLoadGame::ScriptType = type;
    SaveLoadGame::ScriptIndex = index;
}


void ScreenMainGame::update(long delta)
{
    if (mRunScript && mScriptExecutor != NULL)
    {
        mScriptExecutor->process();
        mScriptExecutor->update(delta);
    }
    else if (mCombat->isActive())    // TODO fix this test
    {
        mCombat->update(delta);
    }
    else
    {
        for (int i = 1; i <= 40; i++)
        {
            if (sNPCObjs[i] == NULL)
            {
                continue;
            }
            sNPCObjs[i]->update(delta);
        }
    }
}

void ScreenMainGame::draw(Canvas *canvas)
{
    if (mRunScript && mScriptExecutor != NULL)
    {
        if (mCombat->isActive())
        {
            mCombat->draw(canvas);
        }
        mScriptExecutor->draw(canvas);
    }
    else if (mCombat->isActive())
    {
        mCombat->draw(canvas);
        return;
    }
    else
    {
        drawScene(canvas);
    }
}


void ScreenMainGame::drawScene(Canvas *canvas)
{
    if (sMap != NULL)
    {
        sMap->drawMap(canvas, mMapScreenPos.x, mMapScreenPos.y);
    }

    int playY = 10000;
    bool hasPlayerBeenDrawn = false;
    if (sPlayer != NULL)
    {
        playY = sPlayer->getPosInMap().y;
    }

    std::vector<NPC *> npcs = getSortedNpcObjs();
    for (int i = npcs.size() - 1; i >= 0; --i)
    {
        if (!hasPlayerBeenDrawn && playY < npcs[i]->getPosInMap().y)
        {
            sPlayer->drawWalkingSprite(canvas, &mMapScreenPos);
            hasPlayerBeenDrawn = true;
        }
        npcs[i]->drawWalkingSprite(canvas, &mMapScreenPos);
    }
    if (sPlayer != NULL && !hasPlayerBeenDrawn)
    {
        sPlayer->drawWalkingSprite(canvas, &mMapScreenPos);
    }
    Util::drawSideFrame(canvas);
}

std::vector<NPC *> ScreenMainGame::getSortedNpcObjs()
{
    std::vector<NPC *> arr;
    arr.resize(40);

    int i = 0;
    for (int j = 1; j <= 40; j++)
    {
        if (sNPCObjs[j] != NULL)
        {
            arr[i++] = sNPCObjs[j];
        }
    }

    std::vector<NPC *> arr2;
    arr2.resize(i);

    for (int index = 0; index < i; index++)
    {
        arr2[index] = arr[index];
    }

    arr.clear();
    arr = arr2; //这一步获得干净的没有空位的数据

    // 选择排序
    for (int j = 0; j < i; j++)
    {
        int max = j;
        for (int k = j + 1; k < i; k++)
        {
            if (arr[k]->getPosInMap().y > arr[max]->getPosInMap().y)
            {
                max = k;
            }
        }
        NPC *tmp = arr[j];
        arr[j] = arr[max];
        arr[max] = tmp;
    }
    return arr;
}

void ScreenMainGame::onKeyDown(int key)
{
    if (mRunScript && mScriptExecutor != NULL)
    {
        mScriptExecutor->keyDown(key);
    }
    else if (mCombat->isActive())
    {
        mCombat->onKeyDown(key);
        return;
    }
    else if (sPlayer != NULL)
    {
        switch (key)
        {
        case KEY_LEFT:
            walkLeft();
            break;
        case KEY_RIGHT:
            walkRight();
            break;
        case KEY_UP:
            walkUp();
            break;
        case KEY_DOWN:
            walkDown();
            break;
        case KEY_ENTER:
            triggerSceneObjEvent();
            break;
        }
    }
}

void ScreenMainGame::onKeyUp(int key)
{
    if (mRunScript && mScriptExecutor != NULL)
    {
        mScriptExecutor->keyUp(key);
    }
    else if (mCombat->isActive())
    {
        mCombat->onKeyUp(key);
        return;
    }
    else if (key == KEY_CANCEL)
    {
        GameView::getInstance()->pushScreen(new ScreenGameMainMenu());
    }
}


void ScreenMainGame::gotoAddress(int address)
{
    mScriptExecutor->gotoAddress(address);
    mRunScript = true;
}


void ScreenMainGame::triggerEvent(int eventId)
{
    if (mScriptExecutor != NULL)
    {
        mRunScript = mScriptExecutor->triggerEvent(eventId);
    }
}

void ScreenMainGame::triggerSceneObjEvent()
{
    Player *p = getPlayer();
    int x = p->getPosInMap().x;
    int y = p->getPosInMap().y;
    switch (p->getDirection())
    {
    case East:
        ++x;
        break;
    case North:
        --y;
        break;
    case South:
        ++y;
        break;
    case West:
        --x;
        break;
    }

    // NPC事件
    int npcId = getNpcIdFromPosInMap(x, y);
    if (npcId != 0)
    {
        mRunScript = mScriptExecutor->triggerEvent(npcId);
        return;
    }
    else if (triggerMapEvent(x, y))    // 地图切换
    {
    }
}

bool ScreenMainGame::triggerMapEvent(int x, int y)
{
    if (sMap != NULL && mScriptExecutor != NULL)
    {
        int id = sMap->getEventNum(x, y);
        if (id != 0)
        {
            mScriptExecutor->triggerEvent(id + 40);
            mRunScript = true;
            return true;
        }
    }
    // 未触发地图事件，随机战斗
    mCombat->startNewRandomCombat();
    return false;
}


bool ScreenMainGame::canPlayerWalk(int x, int y)
{
    if (sMap == NULL) return false;
    return sMap->canPlayerWalk(x, y) && getNpcFromPosInMap(x, y) == NULL;
}


void ScreenMainGame::walkLeft()
{
    Point *p = &(sPlayer->getPosInMap());
    triggerMapEvent(p->x - 1, p->y);
    if (canPlayerWalk(p->x - 1, p->y))
    {
        sPlayer->walk(West);
        --mMapScreenPos.x;
        SaveLoadGame::MapScreenX = mMapScreenPos.x;
    }
    else
    {
        sPlayer->walkStay(West);
    }
}

void ScreenMainGame::walkUp()
{
    Point *p = &(sPlayer->getPosInMap());
    triggerMapEvent(p->x, p->y - 1);
    if (canPlayerWalk(p->x, p->y - 1))
    {
        sPlayer->walk(North);
        --mMapScreenPos.y;
        SaveLoadGame::MapScreenY = mMapScreenPos.y;
    }
    else
    {
        sPlayer->walkStay(North);
    }
}

void ScreenMainGame::walkRight()
{
    Point *p = &(sPlayer->getPosInMap());
    triggerMapEvent(p->x + 1, p->y);
    if (canPlayerWalk(p->x + 1, p->y))
    {
        ++mMapScreenPos.x;
        SaveLoadGame::MapScreenX = mMapScreenPos.x;
        sPlayer->walk(East);
    }
    else
    {
        sPlayer->walkStay(East);
    }
}


void ScreenMainGame::walkDown()
{
    Point *p = &(sPlayer->getPosInMap());
    triggerMapEvent(p->x, p->y + 1);
    if (canPlayerWalk(p->x, p->y + 1))
    {
        ++mMapScreenPos.y;
        SaveLoadGame::MapScreenY = mMapScreenPos.y;
        sPlayer->walk(South);
    }
    else
    {
        sPlayer->walkStay(South);
    }
}

void ScreenMainGame::loadMap(int type, int index, int x, int y)
{
    Point *tmpP = NULL;
    if (sPlayer != NULL)
    {
        tmpP = &(sPlayer->getPosOnScreen(&mMapScreenPos));
    }

    if (NULL != sMap)
    {
        delete sMap;
    }

    sMap = (ResMap*)DatLib::getInstance()->getRes(DatLib::RES_MAP, type, index);
    mMapScreenPos.set(x, y);
    if (tmpP != NULL)
    {
        sPlayer->setPosOnScreen(tmpP->x, tmpP->y, &mMapScreenPos);
    }

    SaveLoadGame::MapType = type;
    SaveLoadGame::MapIndex = index;
    SaveLoadGame::MapScreenX = x;
    SaveLoadGame::MapScreenY = y;
}


ResMap *ScreenMainGame::getCurrentMap()
{
    return sMap;
}

void ScreenMainGame::setMapScreenPos(int x, int y)
{
    mMapScreenPos.set(x, y);
}

void ScreenMainGame::createActor(int actorId, int x, int y)
{
    sPlayer = (Player*)DatLib::GetRes(DatLib::RES_ARS, 1, actorId);
    sPlayer->setPosOnScreen(x, y, &mMapScreenPos);
    sPlayerList.push_back(sPlayer);
    sPlayer = sPlayerList[0];
}


void ScreenMainGame::deleteActor(int actorId)
{
    std::vector<Player *>::const_iterator iter;
    for (iter = sPlayerList.begin(); iter != sPlayerList.end(); ++iter)
    {
        if ((*iter)->getIndex() == actorId)
        {
            sPlayerList.erase(iter);
            break;
        }
    }

    if (sPlayerList.size() > 0)
    {
        sPlayer = sPlayerList[0];
    }
    else
    {
        sPlayer = NULL;
    }
}


void ScreenMainGame::createNpc(int id, int npc, int x, int y)
{
    NPC *npcobj = (NPC *)DatLib::GetRes(DatLib::RES_ARS, 2, npc);
    npcobj->setPosInMap(x, y);
    npcobj->setICanWalk(mNpcCanWalk);
    sNPCObjs[id] = npcobj;
}


void ScreenMainGame::deleteNpc(int id)
{
    if (NULL != sNPCObjs[id])
    {
        delete sNPCObjs[id];
    }
    sNPCObjs[id] = NULL;
}


void ScreenMainGame::deleteAllNpc()
{
    for (int i = 0; i < 41; i++)
    {
        if (NULL != sNPCObjs[i])
        {
            delete sNPCObjs[i];
        }
        sNPCObjs[i] = NULL;
    }
}


NPC *ScreenMainGame::getNPC(int id)
{
    return sNPCObjs[id];
}


bool ScreenMainGame::isNpcVisible(NPC *npc)
{
    Point p = npc->getPosOnScreen(&mMapScreenPos);
    return p.x >= 0 && p.x < ResMap::WIDTH &&
           p.y >= 0 && p.y <= ResMap::HEIGHT;
}


bool ScreenMainGame::isNpcVisible(int id)
{
    return isNpcVisible(getNPC(id));
}

NPC *ScreenMainGame::getNpcFromPosInMap(int x, int y)
{
    return sNPCObjs[getNpcIdFromPosInMap(x, y)];
}

int ScreenMainGame::getNpcIdFromPosInMap(int x, int y)
{
    for (int i = 1; i <= 40; i++)
    {
        if (sNPCObjs[i] != NULL && sNPCObjs[i]->getPosInMap().equals(x, y))
        {
            return i;
        }
    }
    return 0;
}

void ScreenMainGame::createBox(int id, int boxIndex, int x, int y)
{
    SceneObj *box = (SceneObj *)DatLib::GetRes(DatLib::RES_ARS, 4, boxIndex);
    box->setPosInMap(x, y);
    sNPCObjs[id] = box;
}


void ScreenMainGame::deleteBox(int id)
{
    sNPCObjs[id] = NULL;
}

Combat * ScreenMainGame::getCombat()
{
    return mCombat;
}

Player * ScreenMainGame::getPlayer()
{
    return sPlayer;
}

Player * ScreenMainGame::getPlayer(int actorId)
{
    for (int i = 0; i < (int)(sPlayerList.size()); i++)
    {
        Player *p = sPlayerList[i];
        if (p->getIndex() == actorId)
        {
            return p;
        }
    }
    return NULL;
}

ScreenMainGame::~ScreenMainGame()
{
    delete mNpcCanWalk;
}

bool ScreenMainGame::NpcCanWalk::canWalk(int x, int y)
{
    bool canwalk = sMap->canWalk(x, y);
    NPC *npc = getNpcFromPosInMap(x, y);
    bool playEuqls = sPlayer->getPosInMap().equals(x, y);

    return canwalk && npc == NULL && !playEuqls;
}

std::vector<Player *> ScreenMainGame::sPlayerList;

NPC * ScreenMainGame::sNPCObjs[41];

Player * ScreenMainGame::sPlayer = NULL;

ResMap * ScreenMainGame::sMap = NULL;

Point ScreenMainGame::mMapScreenPos;

ScriptExecutor * ScreenMainGame::mScriptExecutor = NULL;

bool ScreenMainGame::mRunScript = false;

