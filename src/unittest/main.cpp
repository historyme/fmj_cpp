#include <iostream>
#include <time.h>
#include "SaveLoadStream.h"

using namespace std;

int main()
{
    srand((unsigned int)(time(NULL)));

    while (1)
    {
        int oldInt;
        bool oldBool;
        long oldLong;

        SaveLoadStream *save = new SaveLoadStream();
        oldInt = rand();
        oldBool = rand() % 2 ? true : false;
        oldLong = (long)rand();
        save->writeInt(oldInt);
        save->writeBoolean(oldBool);
        save->writeLong(oldLong);


        int newInt;
        bool newBool;
        long newLong;

        SaveLoadStream *read = new SaveLoadStream();

        memcpy(read->mSaveData, save->mSaveData, save->mLen);
        newInt = read->readInt();
        newBool = read->readBoolean();
        newLong = read->readLong();

        if (newInt != oldInt)
        {
            cout << "error: int " << newInt << "  " << oldInt << endl;
        }

        if (newBool != oldBool)
        {
            cout << "error: bool " << newBool << "  " << oldBool << endl;
        }

        if (oldLong != newLong)
        {
            cout << "error: long " << oldLong << "  " << newLong << endl;
        }


        delete read;
        delete save;
    }

    return 0;
}