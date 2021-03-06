#ifndef _ScriptProcess_h_
#define _ScriptProcess_h_

#include "Operate.h"
#include "OperateDrawOnce.h"
#include "OperateAdapter.h"
#include "ScriptExecutor.h"
#include "ScriptResources.h"
#include "GameView.h"
#include "Global.h"
#include "Character.h"
#include "Direction.h"
#include "NPC.h"
#include "Player.h"
#include "Combat.h"
#include "BaseGoods.h"
#include "TextRender.h"
#include "Util.h"
#include "DatLib.h"
#include "ResBase.h"
#include "ResGut.h"
#include "ResImage.h"
#include "ResSrs.h"

#include "Bitmap.h"
#include "Canvas.h"
#include "Paint.h"
#include "Point.h"
#include "Rect.h"
#include "RectF.h"

#include <iostream>

class ScreenMainGame;

struct Command
{
    /**
    * 得到下一条指令的位置
    *
    * @param code
    *            指令缓冲区
    * @param start
    *            要执行的指令的数据开始位置
    * @return 小于0结束，大于0为下一条指令的位置
    */
    virtual int getNextPos(const char *code, int start) = 0;

    virtual Operate *getOperate(const char *code, const int start) = 0;
};

class ScriptProcess
{
private:
    ScriptProcess();

public:
    static ScriptProcess *getInstance();
    static int get2ByteInt(const char *data, const int start);
    static int get4BytesInt(const char *data, const int start);
    static std::string getStringBytes(const char *data, const int start);

    void setScreenMainGame(ScreenMainGame *screenMainGame)
    {
        sScreenMainGame = screenMainGame;
    }

    void loadScript(ResGut *resGut)
    {
        mScript = resGut;
    }

    bool loadScript(int type, int index);

    ScriptExecutor *getScriptExecutor();

    void initScript();

public:
    static ScriptProcess *instance;
    ResGut *mScript;
    static Command *mCmds[78];
    static ScreenMainGame *sScreenMainGame;
    typedef std::pair<int, int> Int_Pair;
};

#endif
