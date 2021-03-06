#include "ResMap.h"
#include "Tiles.h"

ResMap::ResMap()
{
    mTiles = NULL;
}

ResMap::~ResMap()
{
    if (NULL != mData)
    {
        delete mData;
    }

    if (NULL != mTiles)
    {
        delete mTiles;
    }
}

void ResMap::setData(char *buf, int offset)
{
    mType = buf[offset];
    mIndex = buf[offset + 1];
    mTilIndex = buf[offset + 2];

    int i = 0;
    while (buf[offset + 3 + i] != 0)
    {
        ++i;
    }

    mName = std::string(buf + offset + 3, i);

    mWidth = buf[offset + 0x10];
    mHeight = buf[offset + 0x11];

    int len = mWidth * mHeight * 2;
    mData = new char[len];
    memcpy(mData, buf + offset + 0x12, len);
}

bool ResMap::canWalk(int x, int y)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
    {
        return false;
    }

    int i = y * mWidth + x;
    return (mData[i * 2] & 0x80) != 0;
}

bool ResMap::canPlayerWalk(int x, int y)
{
    return canWalk(x, y) && (x >= 4) && (x < mWidth - 4)
        && (y >= 3) && (y < mHeight - 2);
}

int ResMap::getEventNum(int x, int y)
{
    if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
    {
        return -1;
    }

    int i = y * mWidth + x;
    return (int)mData[i * 2 + 1] & 0xFF;
}


void ResMap::drawMap(Canvas *canvas, int left, int top)
{
    if (mTiles == NULL)
    {
        mTiles = new Tiles(mTilIndex);
    }

    int minY = (std::min)(HEIGHT, mHeight - top);
    int minX = (std::min)(WIDTH, mWidth - left);
    for (int y = 0; y < minY; y++)
    {
        for (int x = 0; x < minX; x++)
        {
            mTiles->draw(canvas, x * Tiles::WIDTH + MAP_LEFT_OFFSET,
                y * Tiles::HEIGHT, getTileIndex(left + x, top + y));
        }
    }
}

void ResMap::drawWholeMap(Canvas *canvas, int x, int y)
{
    if (mTiles == NULL)
    {
        mTiles = new Tiles(mTilIndex);
    }

    for (int ty = 0; ty < mHeight; ty++)
    {
        for (int tx = 0; tx < mWidth; tx++)
        {
            int sx = tx * Tiles::WIDTH + x;
            int sy = ty * Tiles::HEIGHT + y;
            mTiles->draw(canvas, sx, sy, getTileIndex(tx, ty));
            int event = getEventNum(tx, ty);
            if (event != 0)
            {
                //这里需要改变颜色，用来绘制"门",更多的是使用红色底黑色字
                TextRender::getInstance()->drawText(canvas, int_to_string(event), sx, sy);
            }
        }
    }
}


int ResMap::getTileIndex(int x, int y)
{
    int i = y * mWidth + x;
    return (int)mData[i * 2] & 0x7F;
}

