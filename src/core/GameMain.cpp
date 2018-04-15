#include "gameMain.h"

#include <iostream>

#include "GameView.h"
#include "TextRender.h"
#include "DatLib.h"


gameMain::gameMain()
{
    run();
}

gameMain::~gameMain()
{

}

void gameMain::run()
{
    GameView *view = GameView::getInstance();
    view->initView();
    view->run();
}
