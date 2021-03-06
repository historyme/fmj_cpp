#ifndef  _Util_h_
#define  _Util_h_

#include "Canvas.h"
#include "Bitmap.h"
#include "ResImage.h"
#include "DatLib.h"
#include <iostream>

class Util
{

private:
    Util();
    ~Util();
    void init();
public:
    static void InitUtil();

    // 用于showscenename
    static void showInformation(Canvas *canvas, std::string msg);


    // 显示message,每行最多显示8个汉字，最多可显示5行
    static void showMessage(Canvas *canvas, std::string msg);


    // 显示message,每行最多显示8个汉字，最多可显示5行
    static void showMessage(Canvas *canvas, char * msg);

    static void drawSideFrame(Canvas *canvas);

    static Bitmap *getFrameBitmap(int w, int h);

    static void drawTriangleCursor(Canvas *canvas, int x, int y);

    /**
    *
    * @return 画出的num宽度(像素)
    */
    static int drawSmallNum(Canvas *canvas, int num, int x, int y);

    static Bitmap *getSmallSignedNumBitmap(int num);

    static void showMessage(std::string msg, long delay);

public:
    static Bitmap *bmpChuandai;
    static Paint sBlackPaint;
private:
    static Bitmap *bmpTriangleCursor;
    static ResImage *imgSmallNum;
    // 显示message的方框
    static std::vector<Bitmap *> bmpInformationBg;
    // 屏幕两边留白
    static Bitmap *bmpSideFrame;
    // 用于菜单的矩形框，黑框白边
    static Paint *drawFramePaint;

    static Util *Instance;

};

#endif