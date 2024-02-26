#ifndef EFFECTS2D_DATETYPE_H
#define EFFECTS2D_DATETYPE_H

#include "EnumType.h"
struct TouchInfo {
    TouchActionType type;
    float x;
    float y;
};

struct ViewPort {
    ViewPort()
    {
        left = 0;
        bottom = 0;
        width = 0;
        height = 0;
    }

    ViewPort(int l, int b, int w, int h)
    {
        left = l;
        bottom = b;
        width = w;
        height = h;
    }

    ViewPort(ViewPort& v)
    {
        left = v.left;
        bottom = v.bottom;
        width = v.width;
        height = v.height;
    }
    int left;
    int bottom;
    int width;
    int height;
};

#endif //EFFECTS2D_DATETYPE_H
