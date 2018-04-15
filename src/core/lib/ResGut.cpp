#include <iostream>
#include <string.h>

#include "ResGut.h"

ResGut::~ResGut()
{
    delete mScriptData;
    delete mSceneEvent;
}


void ResGut::setData(char *buf, int offset)
{
    mType = buf[offset];
    mIndex = buf[offset + 1];
    mDescription = getString(buf, offset + 2);
    mLength = (((int)buf[offset + 0x19] & 0xFF) << 8) | ((int)buf[offset + 0x18] & 0xFF);
    mNumSceneEvent = (int)buf[offset + 0x1a] & 0xFF;
    mSceneEvent = new int[mNumSceneEvent];
    for (int i = 0; i < mNumSceneEvent; i++) 
    {
        mSceneEvent[i] = ((int)buf[offset + (i << 1) + 0x1c] & 0xFF) << 8
            | ((int)buf[offset + (i << 1) + 0x1b] & 0xFF);
    }
    int len = mLength - mNumSceneEvent * 2 - 3;
    mScriptData = new char[len];
    memcpy(mScriptData, buf + offset + 0x1b + (mNumSceneEvent * 2), len);
    mScriptDataLen = len;
}
