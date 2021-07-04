#ifndef DEFS_H
#define DEFS_H

#include <tiny-fw.h>

#define SCALE_FACTOR 4
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)

#define NUM_POINTS_ASTEROID 11

enum colors {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    BLUE,
    INDIGO,
    VIOLET,
    NUM_COLORS
};

// main.c
extern const int COLORS[NUM_COLORS];

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

void ship_init(ship *s, float x, float y);
void ship_update(ship *s, float dt);
void ship_render(ship *s);

// asteroid.c

enum asteroid_type {
    SMALL,
    MED,
    LARGE,
    NUM_TYPES
};

enum asteroid_model {
    ASTEROID_01,
    ASTEROID_02,
    ASTEROID_03,
    NUM_MODELS
};

typedef struct {
    vec2 vel;
    int size;
    float angle;
    float ang_vel;
    vec2 pos;
    enum asteroid_type type;
    enum asteroid_model model;
} asteroid;

void load_models();
void asteroid_init(asteroid *a);
void asteroid_update(asteroid *a, float dt);
void asteroid_render(asteroid *a);

// utils.c
vec2 wrap_coor(vec2 pos, int w, int h);

#endif // DEFS_H
