#include <iostream>
#include "fmj_ui_sdl1_2.h"
#include "GameMain.h"
#include "Manager.h"
#include <SDL/SDL_draw.h>
#include <SDL/SDL_rotozoom.h>

using namespace std;

SDL_Surface* gpScreen = NULL;

static bool eventFilter();

class SDL1_2HMMathod :public HMMathod
{
public:
    virtual void* InitBitmap(int w, int h, int type);
    virtual void DeleteBitmap(void *haldle);
    virtual void SetPixel(void *haldle, int w, int h, int color);
    virtual void DrawBitmap(void *distHaldle, void *srcHaldle, int x, int y);
    virtual void DrawLine(void *distHaldle, int startX, int startY, int stopX, int stopY, int color);
    virtual void DrawRect(void *distHaldle, int x, int y, int i, int j, int color);
    virtual void FillRect(void *distHaldle, int x, int y, int i, int j, int fillColor);
    virtual void FillColor(void *handle, int color);
    virtual void DrawScreen(void *haldle);
    virtual void pollEvent(void);
};

void* SDL1_2HMMathod::InitBitmap(int w, int h, int type)
{
    SDL_Surface *surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
    return (void *)surface;
}

void SDL1_2HMMathod::SetPixel(void *haldle, int w, int h, int color)
{
    SDL_Surface *surface = (SDL_Surface *)haldle;
    Draw_Pixel(surface, w, h, Uint32(color));
}

void SDL1_2HMMathod::DrawBitmap(void *distHaldle, void *srcHaldle, int x, int y)
{
    SDL_Surface *dist = (SDL_Surface *)distHaldle;
    SDL_Surface *src = (SDL_Surface *)srcHaldle;
    SDL_Rect dstRect;
    dstRect.x = x;
    dstRect.y = y;

    //将紫色抠成透明
    SDL_SetColorKey(src, SDL_SRCCOLORKEY, Manager::sCOLOR_TRANSP);

    SDL_BlitSurface(src, NULL, dist, &dstRect);
}


void SDL1_2HMMathod::DrawLine(void *distHaldle, int startX, int startY, int stopX, int stopY, int color)
{
    SDL_Surface *surface = (SDL_Surface *)distHaldle;
    Draw_Line(surface, startX, startY, stopX, stopY, Uint32(color));
}

void SDL1_2HMMathod::DrawRect(void *distHaldle, int x, int y, int i, int j, int color)
{
    SDL_Surface *surface = (SDL_Surface *)distHaldle;

    Draw_Rect(surface, x, y, i - x, j - y, Uint32(color));
}

void SDL1_2HMMathod::FillRect(void *distHaldle, int x, int y, int i, int j, int fillColor)
{
    SDL_Surface *surface = (SDL_Surface *)distHaldle;
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = i - x;
    rect.h = j - y;

    SDL_FillRect(surface, &rect, Uint32(fillColor));
}

void SDL1_2HMMathod::DrawScreen(void *haldle)
{
    SDL_Rect dst;
    dst.x = 0;
    dst.y = 0;

    SDL_Surface *surface = (SDL_Surface *)haldle;
    SDL_Surface *tmp = zoomSurface(surface, SCALE, SCALE, SMOOTHING_OFF);

    SDL_BlitSurface(tmp, NULL, gpScreen, &dst);
    SDL_Flip(gpScreen);

    SDL_FreeSurface(tmp);
}
 
void SDL1_2HMMathod::pollEvent(void)
{
    eventFilter();
}

void SDL1_2HMMathod::DeleteBitmap(void *haldle)
{
    SDL_Surface *surface = (SDL_Surface *)haldle;
    SDL_FreeSurface(surface);
}
 
void SDL1_2HMMathod::FillColor(void *haldle, int color)
{
    SDL_Surface *surface = (SDL_Surface *)haldle;

    SDL_FillRect(surface, NULL, Uint32(color));
}

int convertKey(int key)
{
    int retKey = -1;
    switch (key)
    {
    case SDLK_RETURN:
        retKey = KEY_ENTER;
        break;
    case SDLK_ESCAPE:
        retKey = KEY_CANCEL;
        break;
    case SDLK_UP:
        retKey = KEY_UP;
        break;
    case SDLK_DOWN:
        retKey = KEY_DOWN;
        break;
    case SDLK_LEFT:
        retKey = KEY_LEFT;
        break;
    case SDLK_RIGHT:
        retKey = KEY_RIGHT;
        break;
    case SDLK_PAGEUP:
        retKey = KEY_PAGEUP;
        break;
    case SDLK_PAGEDOWN:
        retKey = KEY_PAGEDOWN;
        break;
    default:
        break;
    }

    return retKey;
}


static bool eventFilter()
{
    bool bRet = false;
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            bRet = true;
            break;
        case SDL_KEYDOWN:
        {
            int retKey = convertKey(event.key.keysym.sym);
            if (-1 != retKey)
            {
                Manager::getInstance()->onKeyEvent(1, retKey);
            }
            break;
        }
        case SDL_KEYUP:
        {
            int retKey = convertKey(event.key.keysym.sym);
            if (-1 != retKey)
            {
                Manager::getInstance()->onKeyEvent(0, retKey);
            }
            break;
        }
        default:
            break;
        }
    }

    return bRet;
}

// initialise SDL
SDL_Surface* init() 
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) 
    {
        printf("Could not load SDL : %s\n", SDL_GetError());
        exit(-1);
    }
    atexit(SDL_Quit);

    SDL_WM_SetCaption("fmj", NULL);
    SDL_ShowCursor(SDL_DISABLE);

    return SDL_SetVideoMode(SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, 32, SDL_HWSURFACE);
}

void mainGame()
{
    cout << "start the game!" << endl;

    gpScreen = init();

    Manager *man = Manager::getInstance();
    man->setMethod(new SDL1_2HMMathod);
    Manager::sCOLOR_WHITE = SDL_MapRGB(gpScreen->format, 0xFF, 0xFF, 0xFF);
    Manager::sCOLOR_BLACK = SDL_MapRGB(gpScreen->format, 0x00, 0x00, 0x00);
    Manager::sCOLOR_TRANSP = SDL_MapRGB(gpScreen->format, 0xFF, 0x00, 0xFF); //透明色先设置成紫色

    gameMain *game_main = new gameMain();

    delete game_main;
}