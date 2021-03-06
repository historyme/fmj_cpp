#ifndef _ResMap_H_
#define _ResMap_H_

#include <iostream>
#include <algorithm>

#include "ResBase.h"
#include "Global.h"
#include "TextRender.h"
#include "Util.h"
#include "Canvas.h"
#include "common.h"
class Tiles;

class ResMap: public ResBase
{
public:
    ResMap();
    virtual ~ResMap();
    virtual void setData(char *buf, int offset);

    /**
     * 判断地图(x,y)是否可行走
     * @param x
     * @param y
     * @return
     */
    bool canWalk(int x, int y);

    bool canPlayerWalk(int x, int y);

    int getEventNum(int x, int y);


    /**
     * 水平方向 left --- left+WIDTH <br>
     * 竖直方向 top --- top + HEIGHT
     *
     * @param canvas
     * @param left
     *            地图的最左边
     * @param top
     *            地图的最上边
     */
    void drawMap(Canvas *canvas, int left, int top);

    void drawWholeMap(Canvas *canvas, int x, int y);

    int getMapWidth()
    {
        return mWidth;
    }

    int getMapHeight()
    {
        return mHeight;
    }

    std::string getMapName()
    {
        return mName;
    }

private :
    /**
     *
     * @param x
     *            图块的x坐标
     * @param y
     *            图块的y坐标
     * @return map中(x, y)位置的图块在til中的序号
     */
    int getTileIndex(int x, int y);

public :
    /**
     * 横向渲染的地图块总数
     */
    static const int WIDTH = 160 / 16 - 1;

    /**
     * 纵向渲染的地图块总数
     */
    static const int HEIGHT = 96 / 16;

private:
    /**
     * 该地图所用的til图块资源的索引号
     */
    int mTilIndex;

    /**
     * 地图名称
     */
    std::string mName;

    /**
     * 地图宽
     */
    int mWidth;

    /**
     * 地图高
     */
    int mHeight;

    /**
     * 地图数据 两个字节表示一个地图快（从左到右，从上到下）
     * （低字节：最高位1表示可行走，0不可行走。高字节：事件号）
     */
    char *mData;

    /**
     * 地图使用的地图块
     */
    Tiles *mTiles;

};

#endif
