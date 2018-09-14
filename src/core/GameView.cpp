#include "GameView.h"
#include "ScreenMainGame.h"
#include "ScreenSaveLoadGame.h"
#include "common.h"

GameView::GameView()
{
    bInitiated = false;
}


GameView::~GameView()
{
}

void GameView::initRes()
{
    //单例初始化
    TextRender::InitTextRender();
    DatLib::InitDatLib();
    Util::InitUtil();
}

//线程创建后执行的函数
//图形的操作只会在这个线程中实现，所以不需要再图形处理的操作上加锁
void GameView::routine()
{
    Manager *mManager = Manager::getInstance();
    std::vector<BaseScreen *>::const_iterator iter;
    GameView *view = GameView::getInstance();
    Canvas *canvas = view->mCanvas;

    struct  timeval curTime;
    struct  timeval lastTime;
    long leaveTime; 
    long drawTime;

    //等待1ms，让UI先起来
    //SleepMilliSecond(1);

    gettimeofday(&lastTime, NULL);
    while (1)
    {
        gettimeofday(&curTime, NULL);
        leaveTime = (curTime.tv_sec * 1000 + curTime.tv_usec / 1000) -
            (lastTime.tv_sec * 1000 + lastTime.tv_usec / 1000);
        lastTime = curTime;

        //pollevent
        mManager->getMethod()->pollEvent();

        //更新当前屏幕信息
        view->getCurScreen()->update(leaveTime);

        //访问迭代器，找到第一个全屏窗口
        iter = view->mScreenStack.begin();
        for (; iter != view->mScreenStack.end(); ++iter)
        {
            if (!(*iter)->isPopup())
            {
                break;
            }
        }

        // 刷新
        if (canvas != NULL) 
        {
            for (; iter != view->mScreenStack.end(); ++iter)
            {
                (*iter)->draw(canvas);
            }
            view->mPanel->paint();
        }


        gettimeofday(&curTime, NULL);
        drawTime = (curTime.tv_sec * 1000 + curTime.tv_usec / 1000) -
            (lastTime.tv_sec * 1000 + lastTime.tv_usec / 1000);
        if (TIME_GAMELOOP >= drawTime)
        {
            SleepMilliSecond(TIME_GAMELOOP - drawTime);
        }
        else if (drawTime < 0)
        {
            std::cout << "never be there :drawTime = " << drawTime << std::endl;
        }
        if (TIME_GAMELOOP < drawTime)
        {
            std::cout << "lose frame:drawTime = " << drawTime << std::endl;
        }
    }
}

void GameView::run()
{
    //初始化时候显示247
    mScreenStack.push_back(new ScreenAnimation(247));

    bInitiated = true;

    routine();
}

void GameView::changeScreen(int screenCode)
{
    BaseScreen *tmp = NULL;
    switch (screenCode) {
    case SCREEN_DEV_LOGO:
        tmp = new ScreenAnimation(247);
        break;

    case SCREEN_GAME_LOGO:
        tmp = new ScreenAnimation(248);
        break;

    case SCREEN_MENU:
        tmp = new ScreenMenu();
        break;

    case SCREEN_MAIN_GAME:
        tmp = new ScreenMainGame();
        break;

    case SCREEN_GAME_FAIL:
        tmp = new ScreenAnimation(249);
        break;

    case SCREEN_SAVE_GAME:
        tmp = new ScreenSaveLoadGame(ScreenSaveLoadGame::SAVE);
        break;

    case SCREEN_LOAD_GAME:
        tmp = new ScreenSaveLoadGame(ScreenSaveLoadGame::LOAD);
        break;
    }
    if (tmp != NULL) 
    {
        for (int i = mScreenStack.size() - 1; i >= 0; i--)
        {
            delete mScreenStack[i];
            mScreenStack[i] = NULL;
        }
        mScreenStack.clear();

        mScreenStack.push_back(tmp);
    }
}

GameView * GameView::getInstance()
{
    if (NULL == m_GameView)
    {
        m_GameView = new GameView();
    }
    return m_GameView;
}

BaseScreen * GameView::getCurScreen()
{
    return mScreenStack.back();
}

void GameView::popScreen()
{
    delete mScreenStack.back();
    mScreenStack.pop_back();
}

void GameView::pushScreen(BaseScreen *screen)
{
    mScreenStack.push_back(screen);
}

void GameView::initView()
{
    mPanel = new Panel();
    mCanvas = new Canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

    mPanel->setBackground(mCanvas->getBitmap());

    initRes();
}

void GameView::keyDown(int key)
{
    getCurScreen()->onKeyDown(key);
}

void GameView::keyUp(int key)
{
    getCurScreen()->onKeyUp(key);
}


GameView * GameView::m_GameView = NULL;
