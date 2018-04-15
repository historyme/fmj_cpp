#include "OperateBuy.h"
#include "Player.h"
#include "GameView.h"
#include "ScreenMainGame.h"

std::vector<BaseGoods *> * OperateBuy::goodsList = new std::vector < BaseGoods * > ;

OperateBuy::OperateBuy(const char *data, int start)
   :data(data), 
    start(start)
{
    goodsList->clear();
}

bool OperateBuy::process()
{
    for (vector<BaseGoods*>::const_iterator iter = goodsList->begin(); iter != goodsList->end(); ++iter)
    {
        BaseGoods *bg = *iter;
        delete bg;
    }
    goodsList->clear();

    int i = start;
    while (data[i] != 0)
    {
        BaseGoods *g = Player::sGoodsList->getGoods(
            (int)data[i + 1] & 0xff,
            (int)data[i] & 0xff);

        if (g == NULL)
        {
            g = (BaseGoods *)DatLib::GetRes(DatLib::RES_GRS,
                (int)data[i + 1] & 0xff,
                (int)data[i] & 0xff);
            g->setGoodsNum(0);
        }

        goodsList->push_back(g);
        i += 2;
    }
    GameView::getInstance()->pushScreen(new ScreenGoodsList(goodsList, this, ScreenGoodsList::Buy));
    return true;
}

void OperateBuy::draw(Canvas *canvas)
{
    ScreenMainGame::drawScene(canvas);
}

void OperateBuy::onItemSelected(BaseGoods *goods)
{
    if (Player::sMoney < goods->getBuyPrice())
    {
        Util::showMessage("金钱不足!", 1000);
    }
    else
    {
        GameView::getInstance()->pushScreen(new BuyGoodsScreen(goods));
    }
}

Bitmap * OperateBuy::BuyGoodsScreen::bmpBg = NULL;

OperateBuy::BuyGoodsScreen::BuyGoodsScreen(BaseGoods *goods)
{
    bmpBg = Util::getFrameBitmap(136, 55);
    init(goods);
}

OperateBuy::BuyGoodsScreen::~BuyGoodsScreen()
{
    delete bmpBg;
    bmpBg = NULL;
}

void OperateBuy::BuyGoodsScreen::init(BaseGoods *goods)
{
    this->goods = goods;
    buyCnt = 0;
    money = Player::sMoney;
}

void OperateBuy::BuyGoodsScreen::draw(Canvas *canvas)
{
    canvas->drawBitmap(bmpBg, 12, 21);
    TextRender::drawText(canvas, string("金钱:") + int_to_string(money), 15, 24);
    TextRender::drawText(canvas, goods->getName(), 15, 40);
    TextRender::drawText(canvas, string(":") + int_to_string(goods->getGoodsNum()), 93, 40);
    TextRender::drawText(canvas, string("买入个数  :") + int_to_string(buyCnt), 15, 56);
}

void OperateBuy::BuyGoodsScreen::onKeyUp(int key)
{
    if (key == KEY_ENTER)
    {
        Player::sMoney = money;
        if (buyCnt == goods->getGoodsNum() && buyCnt > 0)
        {
            Player::sGoodsList->addGoods(goods->getType(), goods->getIndex(), buyCnt);
        }
        GameView::getInstance()->popScreen();
    }
    else if (key == KEY_CANCEL)
    {
        goods->addGoodsNum(-buyCnt);
        GameView::getInstance()->popScreen();
    }
}

void OperateBuy::BuyGoodsScreen::onKeyDown(int key)
{
    if (key == KEY_UP && goods->getGoodsNum() < 99)
    {
        if (money >= goods->getBuyPrice())
        {
            ++buyCnt;
            goods->addGoodsNum(1);
            money -= goods->getBuyPrice();
        }
        else
        {
            Util::showMessage("金钱不足!", 1000);
        }
    }
    else if (key == KEY_DOWN && buyCnt > 0)
    {
        --buyCnt;
        goods->addGoodsNum(-1);
        money += goods->getBuyPrice();
    }
}
