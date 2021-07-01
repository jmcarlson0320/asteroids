#ifndef DEFS_H
#define DEFS_H

#include <tiny-fw.h>

#define SCALE_FACTOR 4
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)

// ship.c
enum rotate_state {
    ROTATE_LEFT,
    ROTATE_RIGHT,
    ROTATE_STOP
};

typedef struct {
    vec2 *model;
    vec2 *flame;
    float scale;
    float angle;
    vec2 pos;
    vec2 vel;
    float drag;
    float flame_timer;
    int flame_toggle;

    enum rotate_state ctl_rotate;
    int ctl_thrust;
} ship;

void ship_init(ship *s, vec2 *ship_model, vec2 *flame_model, float x, float y);
void ship_update(ship *s, float dt);
void ship_render(ship *s);

// utils.c
vec2 wrap_coor(vec2 pos, int w, int h);

#endif // DEFS_H
