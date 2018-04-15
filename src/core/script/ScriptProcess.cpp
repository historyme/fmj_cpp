#include "ScriptProcess.h"
#include "ScreenMainGame.h"
#include "OperateBuy.h"

struct cmd_music : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_music\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};


struct cmd_loadmap : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_loadmap\n");
        return start + 8;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateDrawOnce(code, start);
    }
    struct _OperateDrawOnce : public OperateDrawOnce
    {
        int type;
        int index;
        int x;
        int y;

        _OperateDrawOnce(const char *code, const int start)
        {
            type = ((int)code[start] & 0xFF) | ((int)code[start + 1] << 8 & 0xFF00);
            index = ((int)code[start + 2] & 0xFF) | ((int)code[start + 3] << 8 & 0xFF00);
            x = ((int)code[start + 4] & 0xFF) | ((int)code[start + 5] << 8 & 0xFF00);
            y = ((int)code[start + 6] & 0xFF) | ((int)code[start + 7] << 8 & 0xFF00);
        }

        virtual  ~_OperateDrawOnce(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->loadMap(type, index, x, y);
            return true;
        }
        virtual void drawOnce(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
        }
    };
};


struct cmd_createactor : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_createactor\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateDrawOnce(code, start);
    }
    struct _OperateDrawOnce : public OperateDrawOnce
    {
        const char *code;
        const int start;
        _OperateDrawOnce(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual  ~_OperateDrawOnce(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->createActor(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2),
                ScriptProcess::get2ByteInt(code, start + 4));
            return true;
        }
        virtual void drawOnce(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
        }
    };
};


struct cmd_deletenpc : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_deletenpc\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }
        virtual ~_OperateAdapter(){}
        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->deleteNpc(ScriptProcess::get2ByteInt(code, start));
            return false;
        }
    };
};


struct cmd_move : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_move\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        long time;
        NPC *npc;
        int dstX;
        int dstY;
        const char *code;
        const int start;

        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            time = 400;
            dstX = ScriptProcess::get2ByteInt(code, start + 2);
            dstY = ScriptProcess::get2ByteInt(code, start + 4);
        }

        virtual ~_Operate(){}

        virtual bool update(long delta)
        {
            time += delta;
            if (time > 100)
            {
                Point *p = &(npc->getPosInMap());
                if (dstX < p->x)
                {
                    npc->Character::walk(West);
                }
                else if (dstX > p->x)
                {
                    npc->Character::walk(East);
                }
                else if (dstY < p->y)
                {
                    npc->Character::walk(North);
                }
                else if (dstY > p->y)
                {
                    npc->Character::walk(South);
                }
                else
                {
                    return false;
                }
                time = 0;
            }
            return true;
        }
        virtual bool process()
        {
            npc = ScriptProcess::sScreenMainGame->getNPC(ScriptProcess::get2ByteInt(code, start));
            return true;
        }
        virtual void onKeyUp(int key)
        {
        }
        virtual void onKeyDown(int key)
        {
        }
        virtual void draw(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
        }
    };
};


struct cmd_callback : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_callback\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->exitScript();
            return false;
        }
    };
};


struct cmd_goto : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_goto\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start));
            return false;
        }
    };
};


struct cmd_if : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_if\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            if (ScriptResources::globalEvents[ScriptProcess::get2ByteInt(code, start)])
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 2));
            }
            return false;
        }
    };
};


struct cmd_set : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_set\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptResources::variables[ScriptProcess::get2ByteInt(code, start)] =
                ScriptProcess::get2ByteInt(code, start + 2);
            return false;
        }
    };
};


struct cmd_say : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_say\n");
        int i = 2;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        int picNum;
        ResImage *headImg;
        std::string text;
        int iOfText;
        int iOfNext;
        bool isAnyKeyDown;
        RectF rWithPic;
        Rect rWithTextT;
        Rect rWithTextB;
        RectF rWithoutPic;
        Rect rWithoutTextT;
        Rect rWithoutTextB;
        Paint paint;

        const char *code;
        const int start;

        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            picNum = ScriptProcess::get2ByteInt(code, start);

            text = ScriptProcess::getStringBytes(code, start + 2);
            iOfText = 0;
            iOfNext = 0;
            isAnyKeyDown = false;
            rWithPic.set(9, 50, 151, 96 - 0.5f); // 有图边框
            rWithTextT.set(44, 58, 145, 75); // 上
            rWithTextB.set(14, 76, 145, 93); // 下
            rWithoutPic.set(9, 55, 151, 96 - 0.5f); // 无图边框
            rWithoutTextT.set(14, 58, 145, 75); // 上
            rWithoutTextB.set(14, 76, 145, 93); // 下

            headImg = NULL;
            if (picNum != 0)
            {
                headImg = (ResImage*)DatLib::getInstance()->getRes(DatLib::RES_PIC, 1, picNum);
            }
            paint.setColor(Manager::sCOLOR_BLACK);
            paint.setStyle(Paint::FILL_AND_STROKE);

        }

        virtual ~_Operate()
        {
            if (NULL != headImg)
            {
                delete headImg;
            }
        }

        virtual bool update(long delta)
        {
            if (isAnyKeyDown)
            {
                if (iOfNext >= (int)(text.size()) - 1) // 最后一位是0
                {
                    return false;
                }
                else
                {
                    iOfText = iOfNext;
                    isAnyKeyDown = false;
                }
            }
            return true;
        }
        virtual bool process()
        {
            iOfText = 0;
            iOfNext = 0;
            return true;
        }
        virtual void onKeyUp(int key)
        {
        } virtual void onKeyDown(int key)
        {
            isAnyKeyDown = true;
        }
        virtual void draw(Canvas *canvas)
        {
            
            if (!ScriptProcess::sScreenMainGame->getCombat()->isActive())
            {
                ScriptProcess::sScreenMainGame->drawScene(canvas);
            }
            if (picNum == 0) // 没头像
            {
                // 画矩形
                paint.setColor(Manager::sCOLOR_WHITE);
                paint.setStyle(Paint::FILL);
                canvas->drawRect(&rWithoutPic, &paint);
                // 画边框
                paint.setColor(Manager::sCOLOR_BLACK);
                paint.setStyle(Paint::STROKE);
                paint.setStrokeWidth(1); //没卵用
                canvas->drawRect(&rWithoutPic, &paint);
                iOfNext = TextRender::drawText(canvas, text, iOfText, &rWithoutTextT);
                iOfNext = TextRender::drawText(canvas, text, iOfNext, &rWithoutTextB);
            }
            else // 有头像
            {
                // 画矩形
                paint.setColor(Manager::sCOLOR_WHITE);
                paint.setStyle(Paint::FILL);
                canvas->drawRect(&rWithPic, &paint);
                // 画边框
                paint.setColor(Manager::sCOLOR_BLACK);
                paint.setStyle(Paint::STROKE);
                paint.setStrokeWidth(1);
                canvas->drawRect(&rWithPic, &paint);
                canvas->drawLine(38, 50, 44, 56);
                canvas->drawLine(44, 56, 151, 56);
                headImg->draw(canvas, 1, 13, 46);
                iOfNext = TextRender::drawText(canvas, text, iOfText, &rWithTextT);
                iOfNext = TextRender::drawText(canvas, text, iOfNext, &rWithTextB);
            }
        }
    };
};


struct cmd_startchapter : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_startchapter\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;

        int type;
        int index;

        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
            type = ((int)code[start] & 0xFF) | ((int)code[start + 1] << 8 & 0xFF);
            index = ((int)code[start + 2] & 0xFF) | ((int)code[start + 3] << 8 & 0xFF);
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->startChapter(type, index);
            return false;
        }
    };
};


struct cmd_screens : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_screens\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->setMapScreenPos(ScriptProcess::get2ByteInt(code, start), ScriptProcess::get2ByteInt(code, start + 2));
            return false;
        }
    };
};


struct cmd_gameover : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_gameover\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            GameView::getInstance()->changeScreen(SCREEN_MENU);
            return false;
        }
    };
};


struct cmd_ifcmp : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_ifcmp\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            if (ScriptResources::variables[ScriptProcess::get2ByteInt(code, start)]
                == ScriptProcess::get2ByteInt(code, start + 2))
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 4));
            }
            return false;
        }
    };
};


struct cmd_add : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_add\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptResources::variables[ScriptProcess::get2ByteInt(code, start)] +=
                ScriptProcess::get2ByteInt(code, start + 2);
            return false;
        }
    };
};


struct cmd_sub : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_sub\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptResources::variables[ScriptProcess::get2ByteInt(code, start)] -=
                ScriptProcess::get2ByteInt(code, start + 2);
            return false;
        }
    };
};

// 伏魔记未用到

struct cmd_setcontrolid : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_setcontrolid\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};


struct cmd_setevent : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_setevent\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptResources::setEvent(ScriptProcess::get2ByteInt(code, start));
            return false;
        }
    };
};


struct cmd_clrevent : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_clrevent\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptResources::clearEvent(ScriptProcess::get2ByteInt(code, start));
            return false;
        }
    };
};

/**
* 序号 种类
*/

struct cmd_buy : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_buy\n");
        int i = 0;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new OperateBuy(code, start);
    }
};


struct cmd_facetoface : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_facetoface\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateDrawOnce(code, start);
    }
    struct _OperateDrawOnce : public OperateDrawOnce
    {
        const char *code;
        const int start;
        _OperateDrawOnce(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual  ~_OperateDrawOnce(){}

        Character *getCharacter(int id)
        {
            if (id == 0)
            {
                return ScriptProcess::sScreenMainGame->getPlayer();
            }
            return ScriptProcess::sScreenMainGame->getNPC(id);
        }
        virtual bool process()
        {
            Character *c1 = getCharacter(ScriptProcess::get2ByteInt(code, start));
            Character *c2 = getCharacter(ScriptProcess::get2ByteInt(code, start + 2));
            Point *p1 = &(c1->getPosInMap());
            Point *p2 = &(c2->getPosInMap());
            if (p1->x > p2->x)
            {
                c2->setDirection(East);
            }
            else if (p1->x < p2->x)
            {
                c2->setDirection(West);
            }
            else
            {
                if (p1->y > p2->y)
                {
                    c2->setDirection(South);
                }
                else if (p1->y < p2->y)
                {
                    c2->setDirection(North);
                }
            }
            return true;
        }
        virtual void drawOnce(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
        }
    };
};


struct cmd_movie : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_movie\n");
        return start + 10;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code,start);
    }
    struct _Operate : public Operate
    {
        int type;
        int index;
        int x;
        int y;
        int ctl;
        int downKey;
        bool isAnyKeyPressed;
        ResSrs *movie;

        const char *code;
        const int start;
        _Operate(const char *code, const int start):
            code(code),
            start(start)
        {
            downKey = 0;
            isAnyKeyPressed = false;
            type = ScriptProcess::get2ByteInt(code, start);
            index = ScriptProcess::get2ByteInt(code, start + 2);
            x = ScriptProcess::get2ByteInt(code, start + 4);
            y = ScriptProcess::get2ByteInt(code, start + 6);
            ctl = ScriptProcess::get2ByteInt(code, start + 8);

            movie = NULL;
        }

        virtual ~_Operate()
        {
            if (NULL !=movie)
            {
                delete movie;
            }
        }

        virtual bool update(long delta)
        {
            if ((ctl == 1 || ctl == 3) && isAnyKeyPressed)
            {
                return false;
            }
            return movie->update(delta);
        }
        virtual bool process()
        {
            if (NULL == movie)
            {
                movie = (ResSrs *)DatLib::getInstance()->getRes(DatLib::RES_SRS, type, index);
                movie->setIteratorNum(5);
                movie->startAni();
            }
            return true;
        }
        virtual void onKeyUp(int key)
        {
            if (key == downKey)
            {
                isAnyKeyPressed = true;
            }
        }
        virtual void onKeyDown(int key)
        {
            downKey = key;
        }
        virtual void draw(Canvas *canvas)
        {
            if (ctl == 2 || ctl == 3)
            {
                ScriptProcess::sScreenMainGame->drawScene(canvas);
            }
            movie->draw(canvas, x, y);
        }
    };
};


struct cmd_choice : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_choice\n");
        int i = 0;
        while (code[start + i] != 0) ++i;
        ++i;
        while (code[start + i] != 0) ++i;
        return start + i + 3;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        std::string choice1;
        std::string choice2;
        Bitmap *bg;
        int bgx;
        int bgy;
        int curChoice;
        int addrOffset;
        bool hasSelect;
        int mLastDownKey;
        const char *code;
        const int start;
        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            choice1 = ScriptProcess::getStringBytes(code, start);
            choice2 = ScriptProcess::getStringBytes(code, start + choice1.length());
            addrOffset = choice1.length() + choice2.length();
            mLastDownKey = -1;

            int w = 0;
            int index = 0;
            std::string tmp;
            if (choice1.length() > choice2.length())
            {
                w = choice1.length() * 8 - 8 + 6;
                tmp.resize(choice1.length());

                for (index = 0; index < (int)(choice2.length()); index++)
                {
                    tmp[index] = choice2[index];
                }

                for (int i = choice2.length() - 1; i < (int)(tmp.length()); i++)
                {
                    tmp[i] = ' ';//这是在干什么？
                }
                tmp[tmp.length() - 1] = 0;
                choice2 = tmp;
            }
            else
            {
                w = choice2.length() * 8 - 8 + 6;
                tmp.resize(choice2.length());

                for (index = 0; index < (int)(choice1.length()); index++)
                {
                    tmp[index] = choice1[index];
                }
                for (int i = choice1.length() - 1; i < (int)(tmp.length()); i++)
                {
                    tmp[i] = ' ';
                }
                tmp[tmp.length() - 1] = 0;
                choice1 = tmp;
            }
            bg = Util::getFrameBitmap(w, 16 * 2 + 6);
            bgx = (160 - bg->getWidth()) / 2;
            bgy = (96 - bg->getHeight()) / 2;
        }

        virtual ~_Operate()
        {
            delete bg;
        }

        virtual bool process()
        {
            curChoice = 0;
            hasSelect = false;
            return true;
        }
        virtual bool update(long delta)
        {
            if (hasSelect)
            {
                if (curChoice == 1)
                {
                    ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + addrOffset));
                }
                return false;
            }
            return true;
        }
        virtual void onKeyUp(int key)
        {
            if (key == KEY_ENTER && mLastDownKey == key)
            {
                hasSelect = true;
            }
        }

        virtual void onKeyDown(int key)
        {
            if (key == KEY_DOWN || key == KEY_UP ||
                key == KEY_LEFT || key == KEY_RIGHT)
            {
                curChoice = 1 - curChoice;
            }
            mLastDownKey = key;
        }
        virtual void draw(Canvas *canvas)
        {
            //ScriptProcess::sScreenMainGame->drawScene(canvas);
            //canvas->drawBitmap(bg, bgx, bgy, NULL);
            //if (curChoice == 0)
            //{
            //    TextRender::drawSelText(canvas, choice1, bgx + 3, bgy + 3);
            //    TextRender::drawText(canvas, choice2, bgx + 3, bgy + 3 + 16);
            //}
            //else
            //{
            //    TextRender::drawText(canvas, choice1, bgx + 3, bgy + 3);
            //    TextRender::drawSelText(canvas, choice2, bgx + 3, bgy + 3 + 16);
            //}
        }
    };
};


struct cmd_createbox : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_createbox\n");
        return start + 8;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->createBox(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2),
                ScriptProcess::get2ByteInt(code, start + 4),
                ScriptProcess::get2ByteInt(code, start + 6));
            return false;
        }
    };
};


struct cmd_deletebox : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_deletebox\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}
 
        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->deleteBox(ScriptProcess::get2ByteInt(code, start));
            return false;
        }
    };
};


struct cmd_gaingoods : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_gaingoods\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        BaseGoods *goods;
        std::string msg;
        long time;
        bool isAnyKeyPressed;
        int downKey;

        const char *code;
        const int start;
        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            goods = (BaseGoods *)DatLib::GetRes(DatLib::RES_GRS,
                ScriptProcess::get2ByteInt(code, start), ScriptProcess::get2ByteInt(code, start + 2));
            msg = std::string("获得:") + goods->getName();
        }

        virtual ~_Operate()
        {
            delete goods;
        }

        virtual bool process()
        {
            goods->setGoodsNum(1);
            Player::sGoodsList->addGoods(goods->getType(), goods->getIndex());
            time = 0;
            isAnyKeyPressed = false;
            downKey = -1;
            return true;
        }
        virtual bool update(long delta)
        {
            time += delta;
            if (time > 1000 || isAnyKeyPressed)
            {
                return false;
            }
            return true;
        }
        virtual void onKeyUp(int key)
        {
            if (key == downKey)
            {
                isAnyKeyPressed = true;
            }
        }
        virtual void onKeyDown(int key)
        {
            downKey = key;
        }
        virtual void draw(Canvas *canvas)
        {
            Util::showMessage(canvas, msg);
        }
    };
};


struct cmd_initfight : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_initfight\n");
        return start + 22;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            std::vector<int> arr;
            arr.clear();
            for (int i = 0; i < 8; i++)
            {
                arr.push_back(ScriptProcess::get2ByteInt(code, start + i * 2));
            }
            ScriptProcess::sScreenMainGame->getCombat()->initFight(arr,
                ScriptProcess::get2ByteInt(code, start + 16),
                ScriptProcess::get2ByteInt(code, start + 18), 
                ScriptProcess::get2ByteInt(code, start + 20));
            return false;
        }
    };
};


struct cmd_fightenable : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_fightenable\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->getCombat()->fightEnable();
            return false;
        }
    };
};


struct cmd_fightdisenable : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_fightdisenable\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }
        
        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->getCombat()->fightDisable();
            return false;
        }
    };
};


struct cmd_createnpc : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_createnpc\n");
        return start + 8;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->createNpc(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2),
                ScriptProcess::get2ByteInt(code, start + 4),
                ScriptProcess::get2ByteInt(code, start + 6));
            return false;
        }
    };
};


struct cmd_enterfight : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_enterfight\n");
        return start + 30;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        // EnterFight
        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 28)); // win the fight
            int monstersType[3] = {
                ScriptProcess::get2ByteInt(code, start + 2),
                ScriptProcess::get2ByteInt(code, start + 4),
                ScriptProcess::get2ByteInt(code, start + 6)
            };
            int scr[3] = {
                ScriptProcess::get2ByteInt(code, start + 8),
                ScriptProcess::get2ByteInt(code, start + 10),
                ScriptProcess::get2ByteInt(code, start + 12)
            };
            int evtRnds[3] = {
                ScriptProcess::get2ByteInt(code, start + 14),
                ScriptProcess::get2ByteInt(code, start + 16),
                ScriptProcess::get2ByteInt(code, start + 18)
            };
            int evts[3] = {
                ScriptProcess::get2ByteInt(code, start + 20),
                ScriptProcess::get2ByteInt(code, start + 22),
                ScriptProcess::get2ByteInt(code, start + 24)
            };

            int lossto = ScriptProcess::get2ByteInt(code, start + 26);
            int winto = ScriptProcess::get2ByteInt(code, start + 28);
            ScriptProcess::sScreenMainGame->getCombat()->enterFight(ScriptProcess::get2ByteInt(code, start), monstersType, scr, evtRnds, evts, lossto, winto);
            ScriptProcess::sScreenMainGame->exitScript();
            return false;
        }
    };
};


struct cmd_deleteactor : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_deleteactor\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->deleteActor(ScriptProcess::get2ByteInt(code, start));
            return false;
        }
    };
};


struct cmd_gainmoney : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_gainmoney\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            Player::sMoney += ScriptProcess::get4BytesInt(code, start);
            return false;
        }
    };
};


struct cmd_usemoney : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_usemoney\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            Player::sMoney -= ScriptProcess::get4BytesInt(code, start);
            return false;
        }
    };
};


struct cmd_setmoney : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_setmoney\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            Player::sMoney = ScriptProcess::get4BytesInt(code, start);
            return false;
        }
    };
};


//struct cmd_learnmagic : public Command
//{
//
//    virtual int getNextPos(const char *code, int start)
//    {
//        printf("cmd_learnmagic\n");
//        return start + 6;
//    }
//
//    virtual Operate *getOperate(const char *code, const int start)
//    {
//        return new _Operate();
//    }
//    struct _Operate : public Operate
//    {
//        bool isAnyKeyDown;
//        long timeCnt;
//        virtual bool update(long delta)
//        {
//            timeCnt += delta;
//            return timeCnt < 1000 && !isAnyKeyDown;
//        }
//        virtual bool process()
//        {
//            isAnyKeyDown = false;
//            timeCnt = 0;
//            return true;
//        }
//        virtual void onKeyUp(int key)
//        {
//        } virtual void onKeyDown(int key)
//        {
//        } virtual void draw(Canvas *canvas) // TODO fix the test
//        {
//            TextRender::drawText(canvas, "学会了魔法:", 0, 0);
//            TextRender::drawText(canvas, "actorId:" + ScriptProcess::get2ByteInt(code, start)
//                + "t" + ScriptProcess::get2ByteInt(code, start + 2)
//                + "i" + ScriptProcess::get2ByteInt(code, start + 4), 0, 16);
//        }
//    };
//};


struct cmd_sale : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_sale\n");
        return start;
    }

    virtual Operate *getOperate(char *code, int start)
    {
//        return new OperateSale();
    }
};


struct cmd_npcmovemod : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_npcmovemod\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->getNPC(ScriptProcess::get2ByteInt(code, start))->setCharacterState(ScriptProcess::get2ByteInt(code, start + 2));
            return false;
        }
    };
};


struct cmd_message : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_message\n");
        int i = 0;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        std::string msg;
        int downKey;
        bool isAnyKeyDown;

        const char *code;
        const int start;
        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            msg = ScriptProcess::getStringBytes(code, start);
        }

        virtual ~_Operate(){}

        virtual bool process()
        {
            downKey = -1;
            isAnyKeyDown = false;
            return true;
        }
        virtual bool update(long delta)
        {
            return !isAnyKeyDown;
        }
        virtual void onKeyUp(int key)
        {
            if (downKey == key)
            {
                isAnyKeyDown = true;
            }
        }
        virtual void onKeyDown(int key)
        {
            downKey = key;
        }
        virtual void draw(Canvas *canvas)
        {
            Util::showMessage(canvas, msg);
        }
    };
};


struct cmd_deletegoods : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_deletegoods\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            bool r = Player::sGoodsList->deleteGoods(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2));
            if (!r)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 2));
            }
            return false;
        }
    };
};


struct cmd_resumeactorhp : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_resumeactorhp\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            Player *p = ScriptProcess::sScreenMainGame->getPlayer(ScriptProcess::get2ByteInt(code, start));
            if (p != NULL)
            {
                p->setHP(p->getMaxHP() * ScriptProcess::get2ByteInt(code, start + 2) / 100);
            }
            return false;
        }
    };
};


//struct cmd_actorlayerup : public Command
//{
//
//    virtual int getNextPos(const char *code, int start)
//    {
//        printf("cmd_actorlayerup\n");
//        return start + 4;
//    }
//
//    virtual Operate *getOperate(const char *code, const int start)
//    {
//        return new _Operate();
//    }
//    struct _Operate : public Operate // TODO
//    {
//        bool exit = false;
//        virtual bool update(long delta)
//        {
//            return !exit;
//        }
//        virtual bool process()
//        {
//            return true;
//        }
//        virtual void onKeyUp(int key)
//        {
//            if (key == KEY_CANCEL)
//            {
//                exit = true;
//            }
//        }
//        virtual void onKeyDown(int key)
//        {
//        } virtual void draw(Canvas *canvas)
//        {
//            TextRender::drawText(canvas, "cmd_actorlayerup", 10, 20);
//            TextRender::drawText(canvas, "press cancel to continue", 0, 40);
//        }
//    };
//};


struct cmd_boxopen : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_boxopen\n");
        return start + 2;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            NPC *box = ScriptProcess::sScreenMainGame->getNPC(ScriptProcess::get2ByteInt(code, start));
            if (box != NULL)
            {
                box->setStep(1);
            }
            return false;
        }
    };
};


struct cmd_delallnpc : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_delallnpc\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->deleteAllNpc();
            return false;
        }
    };
};


struct cmd_npcstep : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_npcstep\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        long time;
        long interval;
        int id;
        int faceto;
        int step;

        const char *code;
        const int start;
        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            time = 0;
            interval = 0;
            id = ScriptProcess::get2ByteInt(code, start); // 0为主角
            faceto = ScriptProcess::get2ByteInt(code, start + 2);
            step = ScriptProcess::get2ByteInt(code, start + 4);
        }

        virtual ~_Operate(){}

        virtual bool update(long delta)
        {
            time += delta;
            return time < interval;
        }
        virtual bool process()
        {
            time = 0;
            Direction d = South;
            switch (faceto) // 与资源文件里的不一样
            {
            case 0:
                d = North;
                break;
            case 1:
                d = East;
                break;
            case 2:
                d = South;
                break;
            case 3:
                d = West;
                break;
            }
            if (id == 0)
            {
                Player *p = ScriptProcess::sScreenMainGame->getPlayer();
                p->setDirection(d);
                p->setStep(step);
                interval = 300;
            }
            else
            {
                // TODO npc's step
                NPC *npc = ScriptProcess::sScreenMainGame->getNPC(id);
                npc->setDirection(d);
                npc->setStep(step);
                if (ScriptProcess::sScreenMainGame->isNpcVisible(npc))
                {
                    interval = 300;
                }
                else
                {
                    interval = 0;
                }
            }
            return true;
        }
        virtual void onKeyUp(int key)
        {
        } virtual void onKeyDown(int key)
        {
        } virtual void draw(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
        }
    };
};


struct cmd_setscenename : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_setscenename\n");
        int i = 0;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptProcess::sScreenMainGame->setSceneName(ResBase::getString(code, start));
            return false;
        }
    };
};


struct cmd_showscenename : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_showscenename\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        long time;
        std::string text;
        bool isAnyKeyDown;

        const char *code;
        const int start;
        _Operate(const char *code, const int start):
            code(code),
            start(start)
        {
            time = 0;
            isAnyKeyDown = false;
        }

        virtual ~_Operate(){}

        virtual bool update(long delta)
        {
            time += delta;
            if (time > 100 && isAnyKeyDown)
            {
                isAnyKeyDown = false;
                return false;
            }
            return time < 1000;
        }
        virtual bool process()
        {
            text = ScriptProcess::sScreenMainGame->getSceneName();
            return true;
        }
        virtual void onKeyUp(int key)
        {
        } 
        virtual void onKeyDown(int key)
        {
            isAnyKeyDown = true;
        }
        virtual void draw(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
            Util::showInformation(canvas, text);
        }
    };
};


struct cmd_showscreen : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_showscreen\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateDrawOnce(code, start);
    }
    struct _OperateDrawOnce : public OperateDrawOnce
    {
        const char *code;
        const int start;

        _OperateDrawOnce(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual  ~_OperateDrawOnce(){}

        virtual bool process()
        {
            return true;
        }
        virtual void drawOnce(Canvas *canvas)
        {
            ScriptProcess::sScreenMainGame->drawScene(canvas);
        }
    };
};


struct cmd_usegoods : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_usegoods\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            bool b = Player::sGoodsList->deleteGoods(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2));
            if (!b)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 4));
            }
            return false;
        }
    };
};

// 伏魔记未用到

struct cmd_attribtest : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_attribtest\n");
        return start + 10;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 伏魔记未用到

struct cmd_attribset : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_attribset\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 伏魔记未用到

struct cmd_attribadd : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_attribadd\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};


struct cmd_showgut : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_showgut\n");
        int i = 4;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _Operate(code, start);
    }
    struct _Operate : public Operate
    {
        ResImage *imgTop;
        ResImage *imgBottom;
        std::string text;
        bool goon;
        long interval;
        long timeCnt;
        int step;
        int curY;
        Rect rect;

        const char *code;
        const int start;

        _Operate(const char *code, const int start) :
            code(code),
            start(start)
        {
            goon = true;
            interval = 50;
            timeCnt = 0;
            step = 1;

            int top = ((int)code[start] & 0xFF) | ((int)code[start + 1] << 8 & 0xFF00);
            int btm = ((int)code[start + 2] & 0xFF) | ((int)code[start + 3] << 8 & 0xFF00);
            imgTop = (ResImage *)DatLib::getInstance()->getRes(DatLib::RES_PIC, 5, top);
            imgBottom = (ResImage*)DatLib::getInstance()->getRes(DatLib::RES_PIC, 5, btm);
            text = ResBase::getString(code, start + 4);
            curY = imgBottom != NULL ? 96 - imgBottom->getHeight() : 96;
            rect.set(0, imgTop != NULL ? imgTop->getHeight() : 0, 160, curY);
        }

        virtual ~_Operate()
        {
            delete imgBottom;
            delete imgTop;
        }

        virtual bool process()
        {
            goon = true;
            interval = 50;
            timeCnt = 0;
            step = 1;
            curY = imgBottom != NULL ? 96 - imgBottom->getHeight() : 96;
            return true;
        }
        virtual bool update(long delta)
        {
            if (!goon) return false;
            timeCnt += delta;
            if (timeCnt >= interval)
            {
                timeCnt = 0;
                curY -= step;
            }
            return true;
        }
        virtual void onKeyUp(int key)
        {
            if (key == KEY_CANCEL)
            {
                goon = false;
            }
            step = 1;
            interval = 50;
        }
        virtual void onKeyDown(int key)
        {
            step = 3;
            interval = 20;
        }
        virtual void draw(Canvas *canvas)
        {
            canvas->drawColor(Manager::sCOLOR_WHITE);
            int e = TextRender::drawText(canvas, text, &rect, curY);
            if (e != 1 && e != 2)
            {
                goon = false;
            }
            if (imgTop != NULL)
            {
                imgTop->draw(canvas, 1, 0, 0);
            }
            if (imgTop != NULL)
            {
                imgBottom->draw(canvas, 1, 0, 96 - imgBottom->getHeight());
            }
        }
    };
};


struct cmd_usegoodsnum : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_usegoodsnum\n");
        return start + 8;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            bool b = Player::sGoodsList->useGoodsNum(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2), ScriptProcess::get2ByteInt(code, start + 4));
            if (!b)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 6));
            }
            return false;
        }
    };
};


struct cmd_randrade : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_randrade\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            Random r;
            int randNum = r.nextInt(1000);
            if (randNum <= ScriptProcess::get2ByteInt(code, start))
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 2));
            }
            return false;
        }
    };
};

// 0-6中用到

struct cmd_menu : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_menu\n");
        int i = 2;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};


struct cmd_testmoney : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_testmoney\n");
        return start + 6;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            if (Player::sMoney < ScriptProcess::get4BytesInt(code, start))
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 4));
            }
            return false;
        }
    };
};

// 伏魔记未用到

struct cmd_callchapter : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_callchapter\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};


struct cmd_discmp : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_discmp\n");
        return start + 8;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            int var = ScriptResources::variables[ScriptProcess::get2ByteInt(code, start)];
            int num = ScriptProcess::get2ByteInt(code, start + 2);
            if (var < num)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 4));
            }
            else if (var > num)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 6));
            }
            return false;
        }
    };
};


struct cmd_return : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_return\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 伏魔记未用到

struct cmd_timemsg : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_timemsg\n");
        int i = 2;
        while (code[start + i] != 0) ++i;
        return start + i + 1;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 0-6

struct cmd_disablesave : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_disablesave\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 0-6

struct cmd_enablesave : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_enablesave\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 伏魔记未用到

struct cmd_gamesave : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_gamesave\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};

// 伏魔记未用到

struct cmd_seteventtimer : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_seteventtimer\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;
    }
};


struct cmd_enableshowpos : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_enableshowpos\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL; //TODO
    }
};


struct cmd_disableshowpos : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_disableshowpos\n");
        return start;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return NULL;//TODO
    }
};


struct cmd_setto : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_setto\n");
        return start + 4;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            ScriptResources::variables[ScriptProcess::get2ByteInt(code, start + 2)] =
                ScriptResources::variables[ScriptProcess::get2ByteInt(code, start)];
            return false;
        }
    };
};


struct cmd_testgoodsnum : public Command
{

    virtual int getNextPos(const char *code, int start)
    {
        printf("cmd_testgoodsum\n");
        return start + 10;
    }

    virtual Operate *getOperate(const char *code, const int start)
    {
        return new _OperateAdapter(code, start);
    }
    struct _OperateAdapter : public OperateAdapter
    {
        const char *code;
        const int start;
        _OperateAdapter(const char *code, const int start) :
            code(code),
            start(start)
        {
        }

        virtual ~_OperateAdapter(){}

        virtual bool process()
        {
            int goodsnum = Player::sGoodsList->getGoodsNum(ScriptProcess::get2ByteInt(code, start),
                ScriptProcess::get2ByteInt(code, start + 2));
            int num = ScriptProcess::get2ByteInt(code, start + 4);
            if (goodsnum == num)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 6));
            }
            else if (goodsnum > num)
            {
                ScriptProcess::sScreenMainGame->gotoAddress(ScriptProcess::get2ByteInt(code, start + 8));
            }
            return false;
        }
    };
};


ScriptProcess::ScriptProcess()
{

    Command *Cmds[78] =
    {
        new cmd_music(), //0
        new cmd_loadmap(),
        new cmd_createactor(),
        new cmd_deletenpc(),
        NULL,
        NULL,
        new cmd_move(),
        NULL,
        NULL,
        new cmd_callback(),
        new cmd_goto(), //10
        new cmd_if(),
        new cmd_set(),
        new cmd_say(),
        new cmd_startchapter(),
        NULL,
        new cmd_screens(),
        NULL,
        NULL,
        NULL,
        new cmd_gameover(),//20
        new cmd_ifcmp(),
        new cmd_add(),
        new cmd_sub(),
        new cmd_setcontrolid(),
        NULL,
        new cmd_setevent(),
        new cmd_clrevent(),
        new cmd_buy(),
        new cmd_facetoface(),
        new cmd_movie(), //30
        new cmd_choice(),
        new cmd_createbox(),
        new cmd_deletebox(),
        new cmd_gaingoods(),
        new cmd_initfight(),
        new cmd_fightenable(),
        new cmd_fightdisenable(),
        new cmd_createnpc(),
        new cmd_enterfight(),
        new cmd_deleteactor(), //40
        new cmd_gainmoney(),
        new cmd_usemoney(),
        new cmd_setmoney(),
        NULL,//new cmd_learnmagic(),
        NULL,//new cmd_sale(),
        new cmd_npcmovemod(),
        new cmd_message(),
        new cmd_deletegoods(),
        new cmd_resumeactorhp(),
        NULL,//new cmd_actorlayerup(), //50
        new cmd_boxopen(),
        new cmd_delallnpc(),
        new cmd_npcstep(),
        new cmd_setscenename(),
        new cmd_showscenename(),
        new cmd_showscreen(),
        new cmd_usegoods(),
        new cmd_attribtest(),
        new cmd_attribset(),
        new cmd_attribadd(),
        new cmd_showgut(), //61//开场
        new cmd_usegoodsnum(),
        new cmd_randrade(),
        new cmd_menu(),
        new cmd_testmoney(),
        new cmd_callchapter(),
        new cmd_discmp(),
        new cmd_return(),
        new cmd_timemsg(),
        new cmd_disablesave(),
        new cmd_enablesave(),
        new cmd_gamesave(),
        new cmd_seteventtimer(),
        new cmd_enableshowpos(),
        new cmd_disableshowpos(),
        new cmd_setto(),
        new cmd_testgoodsnum(),
    };

    for (int i = 0; i < 78; i++)
    {
        mCmds[i] = Cmds[i];
    }

    mScript = NULL;
}


ScriptProcess *ScriptProcess::getInstance()
{
    if (instance == NULL)
    {
        instance = new ScriptProcess();
    }
    return instance;
}

int ScriptProcess::get2ByteInt(const char *data, const int start)
{
    return ((int)data[start] & 0xFF) | ((int)data[start + 1] << 8 & 0xFF00);
}

int ScriptProcess::get4BytesInt(const char *data, const int start)
{
    return ((int)data[start] & 0xFF) | ((int)data[start + 1] << 8 & 0xFF00)
           | ((int)data[start + 2] << 16 & 0xFF0000) | ((int)data[start + 3] << 24);
}

std::string ScriptProcess::getStringBytes(const char *data, const int start)
{
    int i = 0;
    while (data[start + i] != 0)
    {
        ++i;
    }

    std::string str(data + start,i+1);

    return str;
}

bool ScriptProcess::loadScript(int type, int index)
{
    mScript = (ResGut *)DatLib::getInstance()->getRes(DatLib::RES_GUT, type, index);
    return mScript != NULL;
}

ScriptExecutor *ScriptProcess::getScriptExecutor()
{
    if (mScript == NULL)
    {
        return NULL;
    }

    char* acCode = mScript->getScriptData();
    int lCodeLen = mScript->getScriptDataLen();
    int pointer = 0;

    std::hash_map<int, int> map;
    std::hash_map<int, int>::const_iterator iter;
    int iOfOper = 0;

    std::vector<Operate *> operateList;

    while (pointer < lCodeLen)
    {
        map.insert(Int_Pair(pointer, iOfOper));
        ++iOfOper;
        int index = acCode[pointer];
        Command *cmd = mCmds[index];
        operateList.push_back(cmd->getOperate(acCode, pointer + 1));
        pointer = cmd->getNextPos(acCode, pointer + 1);
    }

    int *events = mScript->getSceneEvent();
    int eventsLen = mScript->getSceneEventNum();
    std::vector<int> eventIndex;
    eventIndex.resize(eventsLen);
    for (int i = 0; i < eventsLen; i++)
    {
        if (events[i] == 0)
        {
            eventIndex[i] = -1; // 未使用的事件，存在于前40个中
        }
        else
        {
            iter = map.find(events[i] - eventsLen * 2 - 3);
            eventIndex[i] = iter->second;
        }
    }

    return new ScriptExecutor(operateList, eventIndex, map, eventsLen * 2 + 3);
}

void ScriptProcess::initScript()
{
    if (NULL != mScript)
    {
        delete mScript;
        mScript = NULL;
    }
}

Command * ScriptProcess::mCmds[78];
ScriptProcess *ScriptProcess::instance = NULL;
ScreenMainGame *ScriptProcess::sScreenMainGame = NULL;
