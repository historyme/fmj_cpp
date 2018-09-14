#ifndef _mamager_h_
#define _mamager_h_

#include <iostream>

class HMMathod
{
public:
    virtual void* InitBitmap(int w, int h, int type) = 0;
    virtual void DeleteBitmap(void *haldle) = 0;
    virtual void SetPixel(void *haldle, int w, int h, int color) = 0;
    virtual void DrawBitmap(void *distHaldle, void *srcHaldle, int x, int y) = 0;
    virtual void DrawLine(void *distHaldle, int startX, int startY, int stopX, int stopY, int color) = 0;
    virtual void DrawRect(void *distHaldle, int x, int y, int i, int j, int color) = 0;
    virtual void FillRect(void *distHaldle, int x, int y, int i, int j, int fillColor) = 0;
    virtual void FillColor(void *handle, int color) = 0;
    virtual void DrawScreen(void *haldle) = 0;
    virtual void pollEvent(void) = 0;
};


class Manager
{
public:
    Manager();
    ~Manager();

    static Manager *getInstance();

    HMMathod *getMethod() const;
    void setMethod(HMMathod *mathod);

    void onKeyEvent(int type,int key);
private:
    HMMathod *mMathod;
public:
    static long sCOLOR_WHITE; //0xFFFFFFFF 
    static long sCOLOR_BLACK; //0xFF000000 
    static long sCOLOR_TRANSP; //0x00000000
};

#endif