#ifndef _GameView_h_
#define _GameView_h_

#include <iostream>
#include <stack> 

#include "Global.h"
#include "TextRender.h"
#include "DatLib.h"
#include "BaseScreen.h"
#include "ScreenAnimation.h"
#include "ScreenMenu.h"
#include "Panel.h"

class GameView
{
private:
    GameView();
    ~GameView();

public:
    void initView();
    void run();

    void changeScreen(int screenCode);
    static GameView *getInstance();
    void pushScreen(BaseScreen *screen);
    void popScreen();
    BaseScreen *getCurScreen();

    void keyDown(int key);
    void keyUp(int key);

private:
    void initRes();

    void routine();

private:
    //����
    static GameView *m_GameView;

public:
    //����������ֻ����һ������
    Canvas *mCanvas;
    Panel *mPanel;
    std::vector<BaseScreen *> mScreenStack;

    //�Ƿ��Ѿ���ʼ��
    bool bInitiated;
};


#endif