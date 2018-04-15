#ifndef _SceneObj_H_
#define _SceneObj_H_

#include <iostream>
#include "Direction.h"
#include "NPC.h"

class SaveLoadStream;

class SceneObj: public NPC
{
public:
    SceneObj();
    virtual ~SceneObj(){}

    virtual void  setData(char* buf, int offset);

    void write(SaveLoadStream *buff);

    void read(SaveLoadStream *buff);

    virtual void  walk();

    virtual void  walk(Direction d);

    virtual void  walkStay(Direction d);
};

#endif