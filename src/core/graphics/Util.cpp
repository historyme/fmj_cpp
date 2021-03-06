
#include "Util.h"
#include "common.h"
#include "BaseScreen.h"
#include "GameView.h"


Util::Util()
{
    bmpInformationBg.clear();
    bmpSideFrame = NULL;
    bmpTriangleCursor = NULL;
    imgSmallNum = NULL;
    bmpChuandai = NULL;

    sBlackPaint.setColor(Manager::sCOLOR_BLACK);
    sBlackPaint.setStyle(Paint::STROKE);
    sBlackPaint.setStrokeWidth(1);

    init();
}

Util::~Util()
{

}
void Util::InitUtil()
{
    Instance = new Util();
}

void Util::init()
{
    Canvas canvas(NULL);

    Paint paint;
    paint.setColor(Manager::sCOLOR_WHITE);
    paint.setStyle(Paint::FILL_AND_STROKE);

    if (0 == bmpInformationBg.size())
    {
        bmpInformationBg.resize(5);
        for (int i = 0; i < 5; i++)
        {
            bmpInformationBg[i] = Bitmap::createBitmap(138, 23 + 16 * i, ARGB_8888);
            canvas.setBitmap(bmpInformationBg[i]);
            canvas.drawColor(Manager::sCOLOR_BLACK);
            canvas.drawRect(1, 1, 134, 19 + 16 * i,&paint);

            canvas.drawRect(136, 0, 137, 3, &paint);
            canvas.drawLine(0, 21 + 16 * i, 3, 21 + 16 * i, &paint);
            canvas.drawLine(0, 22 + 16 * i, 3, 22 + 16 * i, &paint);
        }
    }

    if (bmpSideFrame == NULL)
    {
        bmpSideFrame = Bitmap::createBitmap(8, 96, ARGB_8888);
        canvas.setBitmap(bmpSideFrame);
        canvas.drawColor(Manager::sCOLOR_WHITE);
        for (int i = 0; i < 8; i += 2)
        {
            canvas.drawLine(i, 0, i, 96);
        }
    }

    if (bmpTriangleCursor == NULL)
    {
        bmpTriangleCursor = Bitmap::createBitmap(7, 13, ARGB_8888);
        canvas.setBitmap(bmpTriangleCursor);
        canvas.drawColor(Manager::sCOLOR_WHITE);
        for (int i = 0; i < 7; ++i)
        {
            canvas.drawLine(i, i, i, 13 - i);
        }
    }

    if (imgSmallNum == NULL)
    {
        imgSmallNum = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 2, 5);
    }

    if (bmpChuandai == NULL)
    {
        bmpChuandai = Bitmap::createBitmap(22, 39,ARGB_8888);
        int b = Manager::sCOLOR_BLACK, w = Manager::sCOLOR_WHITE;
        int pixels[] =
        {
            w, w, w, w, w, w, w, w, w, b, b, b, w, w, w, w, w, w, w, w, w, w,
            w, w, w, b, b, w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w,
            w, w, b, b, b, b, b, w, w, w, w, w, w, w, w, b, b, b, b, b, w, w,
            w, w, b, b, w, w, w, b, b, b, w, w, b, b, b, w, w, w, b, b, w, w,
            w, w, w, b, w, w, b, w, w, w, w, w, w, w, w, w, w, w, b, b, w, w,
            w, w, w, b, w, w, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, w,
            w, w, w, w, w, w, w, b, w, w, w, w, b, b, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b,
            w, w, w, w, w, w, b, b, w, w, w, b, b, b, b, w, b, b, b, b, b, b,
            w, w, w, w, w, w, w, w, w, b, b, b, b, b, b, w, w, w, b, b, b, w,
            w, w, w, w, w, w, b, b, b, b, w, w, b, b, b, w, w, w, w, w, w, w,
            b, b, b, b, b, b, b, b, w, w, w, w, b, b, b, w, w, w, w, w, w, w,
            w, b, b, b, b, w, w, w, w, w, w, w, b, b, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, b, b, b, b, b, b, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, b, b, b, b, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w,
            w, w, w, w, w, w, b, w, w, w, w, b, b, w, w, w, b, w, w, w, w, w,
            w, w, w, b, b, b, b, b, b, b, w, b, b, w, w, b, b, w, w, w, w, w,
            w, w, b, b, w, w, b, w, w, w, w, b, b, w, b, w, w, w, w, w, w, w,
            w, w, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w,
            b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, w, w, w, w, w,
            w, w, w, b, b, b, b, b, b, b, w, b, b, w, b, b, w, w, w, w, w, w,
            w, w, w, b, b, w, b, b, b, b, w, b, b, w, b, b, w, w, w, w, w, w,
            w, w, w, b, w, b, b, w, w, b, w, w, b, w, b, b, w, w, w, w, w, w,
            w, w, w, b, b, b, b, b, b, b, w, w, b, b, b, b, w, w, w, w, w, w,
            w, w, w, b, b, b, b, b, b, b, w, w, b, b, b, w, w, w, w, w, w, w,
            w, w, w, w, b, b, b, b, b, w, w, w, w, b, b, b, w, w, w, w, w, w,
            b, b, b, b, b, w, w, w, w, b, b, b, b, b, b, b, b, b, b, w, w, w,
            w, w, w, b, b, b, w, w, b, b, w, b, b, w, w, b, b, b, b, b, b, b,
            w, w, b, b, w, w, w, w, w, w, b, w, w, w, w, w, b, b, b, b, b, w,
            w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, b, b, w, w,
        };
        bmpChuandai->setPixels(pixels, 0, 22, 0, 0, 22, 39);
    }
}

// 用于showscenename
void Util::showInformation(Canvas *canvas, std::string msg)
{
    canvas->drawBitmap(bmpInformationBg[0], 11, 37);
    TextRender::getInstance()->drawText(canvas, msg, 16, 39);
}

// 显示message,每行最多显示8个汉字，最多可显示5行
void Util::showMessage(Canvas *canvas, std::string msg)
{
    const char * text = msg.c_str();
    int lineNum = msg.length() / 16; // 所需行数
    if (lineNum >= 5) lineNum = 4;
    int textY = 39 - lineNum * 8;
    canvas->drawBitmap(bmpInformationBg[lineNum], 11, textY - 2);
    TextRender::getInstance()->drawText(canvas, text, 0, new Rect(16, textY, 16 + 16 * 8, textY + 16 * lineNum + 16));

}

//// 显示message,每行最多显示8个汉字，最多可显示5行
//void Util::showMessage(Canvas *canvas, char * msg)
//{
//    int lineNum = msg.length / 16;
//    if (lineNum >= 5) lineNum = 4;
//    int textY = 39 - lineNum * 8;
//    canvas->drawBitmap(bmpInformationBg[lineNum], 11, textY - 2);
//    TextRender::getInstance()->drawText(canvas, msg, 0, new Rect(16, textY, 16 + 16 * 8, textY + 16 * lineNum + 16));
//}



void Util::drawSideFrame(Canvas *canvas)
{
    canvas->drawBitmap(bmpSideFrame, 0, 0);
    canvas->drawBitmap(bmpSideFrame, 152, 0);
}

Bitmap *Util::getFrameBitmap(int w, int h)
{
    // 先创建Bitmap
    Bitmap *bmp = Bitmap::createBitmap(w, h, ARGB_8888);
    Canvas tmpC(bmp);
    tmpC.drawColor(Manager::sCOLOR_WHITE);
    tmpC.drawRect(1, 1, w - 2, h - 2, drawFramePaint);
    return bmp;
}


void Util::drawTriangleCursor(Canvas *canvas, int x, int y)
{
    canvas->drawBitmap(bmpTriangleCursor, x, y);
}

/**
*
* @return 画出的num宽度(像素)
*/
int Util::drawSmallNum(Canvas *canvas, int num, int x, int y)
{
    if (num < 0)
    {
        num = -num;
    }
    std::string strNum = int_to_string(num);

    for (int i = 0; i < (int)(strNum.length()); i++)
    {
        imgSmallNum->draw(canvas, strNum[i] - '0' + 1, x, y);
        x += imgSmallNum->getWidth() + 1;
    }

    return strNum.length() * imgSmallNum->getWidth();
}


Bitmap *Util::getSmallSignedNumBitmap(int num)
{
    std::string nums = int_to_string(num > 0 ? num : -num);
    ResImage *sign = (ResImage*)DatLib::GetRes(DatLib::RES_PIC, 2, num > 0 ? 6 : 7);
    Bitmap *bmp = Bitmap::createBitmap(sign->getWidth() + nums.length() * imgSmallNum->getWidth() + 1 + nums.length(),
        imgSmallNum->getHeight(), ARGB_8888);

    Canvas c(bmp);
    c.drawColor(Manager::sCOLOR_TRANSP);//需要先设置成透明的

    sign->draw(&c, 1, 0, 0);

    //TODO 攻击时候显示伤害数字有点模糊，这里分辨率有问题
    int x = sign->getWidth() + 1;
    for (int i = 0; i < nums.length(); i++)
    {
        imgSmallNum->draw(&c, nums[i] - '0' + 1, x, 0);
        x += imgSmallNum->getWidth() + 1;
    }

    delete sign;

    return bmp;
}

void Util::showMessage(std::string msg, long delay)
{

    class UtilShowMessage :public BaseScreen
    {
    public:
        long cnt;
        std::string msg;
        long delay;
        UtilShowMessage(std::string msg, long delay)
        {
            cnt = 0;
            this->delay = delay;
            this->msg = msg;
        }

        virtual void update(long delta)
        {
            cnt += delta;
            if (cnt > delay)
            {
                GameView::getInstance()->popScreen();
            }
        }

        virtual void draw(Canvas *canvas)
        {
            Util::showMessage(canvas, msg);
        }

        virtual void onKeyUp(int key)
        {
        }

        virtual void onKeyDown(int key)
        {
            GameView::getInstance()->popScreen();
        }

        virtual bool isPopup()
        {
            return true;
        }

    };

    GameView::getInstance()->pushScreen(new UtilShowMessage(msg, delay));
}

std::vector<Bitmap *> Util::bmpInformationBg;
Util * Util::Instance;
Paint * Util::drawFramePaint = NULL;
Bitmap * Util::bmpSideFrame = NULL;
Bitmap * Util::bmpTriangleCursor = NULL;
ResImage * Util::imgSmallNum = NULL;
Bitmap * Util::bmpChuandai = NULL;
Paint Util::sBlackPaint;

