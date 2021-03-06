#include "ResImage.h"
#include "Global.h"

void ResImage::setData(char *buf, int offset)
{
    // TODO Auto-generated method stub
    mType = buf[offset];
    mIndex = (int)buf[offset + 1] & 0xFF;
    mWidth = (int)buf[offset + 2] & 0xFF;
    mHeight = (int)buf[offset + 3] & 0xFF;
    mNumber = (int)buf[offset + 4] & 0xFF;
    mTransparent = buf[offset + 5] == 2;

    int len = mNumber * (mWidth / 8 + (mWidth % 8 != 0 ? 1 : 0))
        * mHeight * buf[offset + 5];
    mData = new char[len];
    mDataLen = len;

    memcpy(mData, buf + offset + 6, len);

    createBitmaps();
}



void ResImage::createBitmaps()
{
    mBitmaps.resize(mNumber);
   
    int *tmp = new int[mWidth * mHeight];
    int iOfData = 0;

    if (mTransparent)
    {
        for (int i = 0; i < mNumber; i++)
        {
            int cnt = 0, iOfTmp = 0;
            for (int y = 0; y < mHeight; y++)
            {
                for (int x = 0; x < mWidth; x++)
                {
                    if (((mData[iOfData] << cnt) & 0x80) != 0)
                    {
                        tmp[iOfTmp] = Manager::sCOLOR_TRANSP;
                    }
                    else
                    {
                        tmp[iOfTmp] = ((mData[iOfData] << cnt << 1) & 0x80) != 0 ?
                        Manager::sCOLOR_BLACK : Manager::sCOLOR_WHITE;
                    }
                    ++iOfTmp;
                    cnt += 2;
                    if (cnt >= 8)
                    {
                        cnt = 0;
                        ++iOfData;
                    }
                }

                if (cnt > 0 && cnt <= 7)
                {
                    cnt = 0;
                    ++iOfData;
                }
                if (iOfData % 2 != 0) ++iOfData;
            }
            mBitmaps[i] = Bitmap::createBitmap(tmp, mWidth, mHeight,ARGB_8888);
        } // for mNumber
    }
    else
    {
        // 不透明
        for (int i = 0; i < mNumber; i++)
        {
            int cnt = 0, iOfTmp = 0;
            for (int y = 0; y < mHeight; y++)
            {
                for (int x = 0; x < mWidth; x++)
                {
                    tmp[iOfTmp++] = ((mData[iOfData] << cnt) & 0x80) != 0 ? Manager::sCOLOR_BLACK
                        : Manager::sCOLOR_WHITE;
                    if (++cnt >= 8)
                    {
                        cnt = 0;
                        ++iOfData;
                    }
                }
                if (cnt != 0)
                {
                    // 不足一字节的舍去
                    cnt = 0;
                    ++iOfData;
                }
            }
            mBitmaps[i] = Bitmap::createBitmap(tmp, mWidth, mHeight,ARGB_8888);
        }
    }

    delete tmp;
}


void ResImage::draw(Canvas *canvas, int num, int left, int top)
{
    if (num <= mNumber)
    {
        canvas->drawBitmap(mBitmaps[num - 1], left, top);
    }
    else
    {
        if (mNumber > 0)   // 要改？
        {
            canvas->drawBitmap(mBitmaps[0], left, top);
        }
        else
        {
            //太坑了，用\烫来代替换行
//            TextRender::getInstance()->drawText(canvas, "烫", left, top);
        }
    }
}

ResImage::ResImage()
{
    mData = NULL;
}

ResImage::~ResImage()
{
    if (NULL != mData)
    {
        delete mData;
    }

    for (int i = 0; i < (int)(mBitmaps.size()); i++)
    {
        delete mBitmaps[i];
    }
    mBitmaps.clear();
}

Bitmap *ResImage::getBitmap(int index)
{
   if (index >= mNumber)
   {
       return NULL;
   }

   return mBitmaps[index];
}
