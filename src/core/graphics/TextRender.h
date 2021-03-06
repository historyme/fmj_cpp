#ifndef _TextRender_h_
#define _TextRender_h_

#include <iostream>
#include <fstream>

#include "Global.h"

#include "Bitmap.h"
#include "Canvas.h"

class TextRender
{
private:
    static char *mHZKBuf;
    static char *mASCBuf;
    static int *mPixels;

    static Bitmap *mBmpHzk;
    static Bitmap *mBmpAsc;
    
    static TextRender *mTextRenderInstance;

public:

    TextRender();
    ~TextRender();

    static TextRender *getInstance();
    static void InitTextRender();

    void init();
    static void drawText(Canvas *canvas, std::string &text, int x, int y, bool bInverse = false);
    static void drawSelText(Canvas *canvas, std::string &text, int x, int y);
    static void drawSelText(Canvas *canvas,const char *text, int x, int y);
    static void drawText(Canvas *canvas, const char *text, int x, int y, bool bInverse = false);

    /**
     *
     * @param canvas
     * @param text
     * @param r
     * @param y
     * @return 0,文字都在r.top上方
     * 1,文字在r中
     * 2,文字都在r.bottom下方
     * -1,出错
     */
    static int drawText(Canvas *canvas, std::string & text, Rect *r, int y);

    /**
     * call drawText(Canvas, byte[], int, Rect)
     */
    static int drawText(Canvas *canvas, std::string & text, int start, Rect *r);

    /**
     *
     * @param canvas
     * @param buf
     * @param start buf中第一个要画的字节
     * @param r
     * @return 下一个要画的字节
     */
    static int drawText(Canvas *canvas, const char *buf, int start, Rect *r);

private:
    static Bitmap *getHzk(int offset, bool bInverse = false);

    static Bitmap *getAsc(int offset, bool bInverse = false);

};

#endif