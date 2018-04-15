#ifndef _IEatMedicine_H_
#define _IEatMedicine_H_

#include "Player.h"

struct IEatMedicine
{
    virtual ~IEatMedicine(){}
	virtual void eat(Player *player) = 0;
};

#endif
