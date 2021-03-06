#ifndef _Panel_h_
#define _Panel_h_

#include <iostream>
#include "Global.h"
#include "Bitmap.h"

class Panel
{

public:

    Panel();
    virtual ~Panel();
    void paint();
    void repaint();
    void setBackground(Bitmap *background);
private:
    void draw(Bitmap *background);
private:
    Bitmap *background;
};

#endif