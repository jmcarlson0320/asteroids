#include "defs.h"
#include <stdlib.h>

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

void clear_screen()
{
    draw_fill_rect(0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
}

float dist(vec2 *u, vec2 *v)
{
    vec2 dist_vec;
    vec2_sub(&dist_vec, u, v);
    return vec2_len(&dist_vec);
}

int point_in_circle(vec2 *point, vec2 *origin, float radius)
{
    return (dist(point, origin) < radius);
}

int circle_overlap(vec2 *origin_a, float radius_a, vec2 *origin_b, float radius_b)
{
    return (dist(origin_a, origin_b) < radius_a + radius_b);
}

float rand_float(float min, float max)
{
    return (float) rand() / (float) RAND_MAX * (max - min) + min;
}
