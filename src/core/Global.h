#ifndef _Global_h_
#define _Global_h_

#include "Manager.h"

//#define Scale = 3; //游戏放大缩小倍数
#define  SCREEN_WIDTH  160
#define  SCREEN_HEIGHT  96

#define  MAP_LEFT_OFFSET  8

#define  TIME_GAMELOOP  10

enum
{
    KEY_UP = 1,
    KEY_DOWN = 2,
    KEY_LEFT = 3,
    KEY_RIGHT = 4,
    KEY_PAGEUP = 5,
    KEY_PAGEDOWN = 6,
    KEY_ENTER = 7,
    KEY_CANCEL = 8,
};

enum
{
    SCREEN_DEV_LOGO = 1,
    SCREEN_GAME_LOGO = 2,
    SCREEN_MENU = 3,
    SCREEN_MAIN_GAME = 4,
    SCREEN_GAME_FAIL = 5,
    SCREEN_SAVE_GAME = 6,
    SCREEN_LOAD_GAME = 7,
};

#endif