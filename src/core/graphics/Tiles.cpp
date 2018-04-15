#include "Tiles.h"


Tiles::Tiles(int index)
{
    mTileRes = (ResImage *)DatLib::getInstance()->getRes(DatLib::RES_TIL, 1,
               index);
}

Tiles::~Tiles()
{
    delete mTileRes;
}

void Tiles::draw(Canvas *canvas, int x, int y, int i)
{
    mTileRes->draw(canvas, i + 1, x, y);
}

