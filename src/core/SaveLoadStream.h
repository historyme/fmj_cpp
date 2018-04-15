#pragma once

#include <iostream>
#include <vector>

using namespace std;

#define KB 1024

//游戏保存的数据格式
class SaveLoadStream
{
public:
    SaveLoadStream();
    char mSaveData[100 * KB];
    int mLen;
    int mPos;

    void writeString(std::string &str);
    void writeInt(int num);
    void writeLong(long num);
    void writeBoolean(bool b);
    bool writeCheck();

    bool readCheck();
    std::string readString();
    int readInt();
    long readLong();
    bool readBoolean();  
};
