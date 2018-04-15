#include "SaveLoadStream.h"

SaveLoadStream::SaveLoadStream()
{
    mLen = 0;
    mPos = 0;
    memset(mSaveData, 0x00, sizeof(mSaveData));
}

void SaveLoadStream::writeString(std::string &str)
{
    //string 格式的数据，只有64字节长度，超长的需要另起
    const char *p = str.c_str();
    size_t strLen = strlen(p);

    if (strLen >= 64)
    {
        std::cout << "string too long:" << strLen << std::endl;
        return;
    }

    char *tmp = new char[64];
    memset(tmp, 0x00, 64);
    memcpy(tmp, p, strLen);

    memcpy(mSaveData + mLen, tmp, 64);
    mLen += 64;

    delete tmp;
}

void SaveLoadStream::writeInt(int num)
{
    //int 格式的数据
    int lIntLen = sizeof(int);
    char *tmp = new char[lIntLen];

    int i = 0;
    for (; i < lIntLen; i++)
    {
        tmp[i] = (char)((num >> 8 * i) & 0xFF);
    }

    memcpy(mSaveData + mLen, tmp, lIntLen);
    mLen += lIntLen;

    delete tmp;
}

void SaveLoadStream::writeLong(long num)
{
    int lIntLen = sizeof(long);
    char *tmp = new char[lIntLen];

    int i = 0;
    for (; i < lIntLen; i++)
    {
        tmp[i] = (char)((num >> 8 * i) & 0xFF);
    }

    memcpy(mSaveData + mLen, tmp, lIntLen);
    mLen += lIntLen;

    delete tmp;
}

void SaveLoadStream::writeBoolean(bool b)
{
    int lIntLen = 1;
    char tmp[1];
    if (b)
    {
        tmp[0] = 0x01;
    }
    else
    {
        tmp[0] = 0x00;
    }

    memcpy(mSaveData + mLen, tmp, lIntLen);
    mLen += lIntLen;
}

bool SaveLoadStream::writeCheck()
{
    //TODO 校验
    return true;
}

bool SaveLoadStream::readCheck()
{
    //TODO 校验
    return true;
}

std::string SaveLoadStream::readString()
{
    int strLen = 64;
    char *tmp = new char[strLen];
    memcpy(tmp, mSaveData + mPos, strLen);
    mPos += strLen;

    std::string str = std::string(tmp);
    delete tmp;

    return str;
}

int SaveLoadStream::readInt()
{
    //int 格式的数据
    int data = 0;
    int lIntLen = sizeof(int);
    char *tmp = new char[lIntLen];

    memcpy(tmp, mSaveData + mPos, lIntLen);
    mPos += lIntLen;

    int i = 0;
    for (; i < lIntLen; i++)
    {
        int tmpdat = (int)(tmp[i] & 0xFF);
        data += (tmpdat << 8 * i);
    }

    delete tmp;

    return data;
}

long SaveLoadStream::readLong()
{
    long data = 0;
    int lLen = sizeof(long);
    char *tmp = new char[lLen];

    memcpy(tmp, mSaveData + mPos, lLen);
    mPos += lLen;

    int i = 0;
    for (; i < lLen; i++)
    {
        int tmpdat = (int)(tmp[i] & 0xFF);
        data += (tmpdat << 8 * i);
    }

    delete tmp;

    return data;
}

bool SaveLoadStream::readBoolean()
{
    int lLen = 1;
    char tmp[1];

    memcpy(tmp, mSaveData + mPos, lLen);
    mPos += lLen;

    return (tmp[0] != 0) ? true : false;
}

