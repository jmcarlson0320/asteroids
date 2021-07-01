#include "defs.h"

vec2 wrap_coor(vec2 pos, int w, int h)
{
    vec2 new = pos;
    if (pos.e[X_COOR] < 0) {
        new.e[X_COOR] = WIDTH + pos.e[X_COOR];
    } else if (pos.e[X_COOR] >= w) {
        new.e[X_COOR] = pos.e[X_COOR] - w;
    } else if (pos.e[Y_COOR] < 0) {
        new.e[Y_COOR] = h + pos.e[Y_COOR];
    } else if (pos.e[Y_COOR] >= h) {
        new.e[Y_COOR] = pos.e[Y_COOR] - h;
    }

    return new;
}
