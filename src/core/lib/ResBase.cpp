#include <iostream>

#include "ResBase.h"



std::string ResBase::getString(char *buf, int start)
{
    int i = 0;
    while (buf[start + i] != 0)
    {
        ++i;
    }

    return std::string((char *)(buf + start));
}

std::string ResBase::getString(const char *buf, int start)
{
    int i = 0;
    while (buf[start + i] != 0)
    {
        ++i;
    }

    return std::string((char *)(buf + start));
}


int ResBase::get2BytesInt(char *buf, int start)
{
    return ((int)buf[start] & 0xFF) | ((int)buf[start + 1] << 8 & 0xFF00);
}


int ResBase::get2BytesSInt(char *buf, int start)
{
    int i = ((int)buf[start] & 0xFF) | ((int)buf[start + 1] << 8 & 0x7F00);
    if (((int)buf[start + 1] & 0x80) != 0) {
        return -i;
    }
    return i;
}

int ResBase::get1ByteSInt(char *buf, int start)
{
    int i = (int)buf[start] & 0x7f;
    if (((int)buf[start] & 0x80) != 0) {
        return -i;
    }
    return i;
}

