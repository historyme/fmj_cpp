#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <iostream>

enum Config
{

    // these native values must match up with the enum in SkBitmap.h

    /**
    * Each pixel is stored as a single translucency (alpha) channel.
    * This is very useful to efficiently store masks for instance.
    * No color information is stored.
    * With this configuration, each pixel requires 1 byte of memory.
    */
    ALPHA_8 = 1,

    /**
    * Each pixel is stored on 2 bytes and only the RGB channels are
    * encoded: red is stored with 5 bits of precision (32 possible
    * values), green is stored with 6 bits of precision (64 possible
    * values) and blue is stored with 5 bits of precision.
    *
    * This configuration can produce slight visual artifacts depending
    * on the configuration of the source. For instance, without
    * dithering, the result might show a greenish tint. To get better
    * results dithering should be applied.
    *
    * This configuration may be useful when using opaque bitmaps
    * that do not require high color fidelity.
    */
    RGB_565 = 3,

    /**
    * Each pixel is stored on 2 bytes. The three RGB color channels
    * and the alpha channel (translucency) are stored with a 4 bits
    * precision (16 possible values.)
    *
    * This configuration is mostly useful if the application needs
    * to store translucency information but also needs to save
    * memory.
    *
    * It is recommended to use {@link #ARGB_8888} instead of this
    * configuration.
    *
    * Note: as of {@link android.os.Build.VERSION_CODES#KITKAT},
    * any bitmap created with this configuration will be created
    * using {@link #ARGB_8888} instead.
    *
    * @deprecated Because of the poor quality of this configuration,
    *             it is advised to use {@link #ARGB_8888} instead.
    */
    ARGB_4444 = 4,

    /**
    * Each pixel is stored on 4 bytes. Each channel (RGB and alpha
    * for translucency) is stored with 8 bits of precision (256
    * possible values.)
    *
    * This configuration is very flexible and offers the best
    * quality. It should be used whenever possible.
    */
    ARGB_8888 = 5,
};


class Bitmap 
{
public:
    Bitmap(int width, int height, Config imageType);
//    Bitmap();
    virtual ~Bitmap();
    static Bitmap *createBitmap(int width, int height, Config c);
    static Bitmap *createBitmap(int* tmp, int mWidth, int mHeight, Config argb8888);

    void setPixels(int *pixels, int offset, int stride, int x, int y, int width, int height);

    int getWidth()
    {
        return mWidth;
    }

    int getHeight()
    {
        return mHeight;
    }

public:
    //保存外部实现的handle
    void *handle;

    int mWidth;
    int mHeight;
};

#endif _BITMAP_H_