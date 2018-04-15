#include "Bitmap.h"
#include "Manager.h"

Bitmap::Bitmap(int width, int height, Config imageType):
    mWidth(width),
    mHeight(height)
{

    Manager *mManager = Manager::getInstance();
    handle = mManager->getMethod()->InitBitmap(width, height, imageType);
}

//Bitmap::Bitmap()
//{
//
//}

Bitmap::~Bitmap()
{
    Manager *mManager = Manager::getInstance();
    mManager->getMethod()->DeleteBitmap(handle);
}

Bitmap * Bitmap::createBitmap(int width, int height, Config c)
{
    return new Bitmap(width, height, c);
}

Bitmap * Bitmap::createBitmap(int* tmp, int mWidth, int mHeight, Config argb8888)
{
    Bitmap *b = new Bitmap(mWidth, mHeight, argb8888);
    b->setPixels(tmp, 0, mWidth, 0, 0, mWidth, mHeight);
    return b;
}

void Bitmap::setPixels(int *pixels, int offset, int stride, int x, int y, int width, int height)
{
    Manager *mManager = Manager::getInstance();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            mManager->getMethod()->SetPixel(handle, j, i, pixels[i*width + j]);
        }
    }
}

