#ifndef _SaveLoadGame_h_
#define _SaveLoadGame_h_

#include <iostream>

using namespace std;

class SaveLoadStream;

class SaveLoadGame
{
public:
    /**
     * 是否开始新游戏
     */
    static bool startNewGame;

    /**
     * 当前地图编号
     */
    static int MapType;
    static int MapIndex;

    /**
     * 屏幕左上角在地图中的位置
     */
    static int MapScreenX;
    static int MapScreenY;

    /**
     * 当前脚本编号
     */
    static int ScriptType;
    static int ScriptIndex;

    /**
     *  场景名称
     */
    static std::string SceneName;

public:
    SaveLoadGame();
    virtual ~SaveLoadGame();

    static bool write(SaveLoadStream *buff);
    static bool read(SaveLoadStream *buff);

private:
    static void readNpcObjs(SaveLoadStream *buff);
    static void writeNpcObjs(SaveLoadStream *buff);
};

#endif