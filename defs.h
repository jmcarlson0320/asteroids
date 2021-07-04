#ifndef DEFS_H
#define DEFS_H

#include <tiny-fw.h>
#include "particle.h"
#include "list.h"

#define SCALE_FACTOR 4
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)
#define NUM_POINTS_ASTEROID 11
#define MAX_BULLETS 4
#define BULLET_LIFETIME 1
#define ACTIVE 1
#define INACTIVE 0

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

enum game_states {
    TITLE,
    PLAY,
    RESET,
    GAME_OVER,
    SHUTDOWN,
    NUM_STATES
};

enum game_events {
    START_GAME,
    DESTROYED,
    TIMER,
    LEVEL_CLEARED,
    DEFEATED,
    INITIALS_ENTERED,
    EXIT,
    NO_EVENT,
    NUM_EVENTS
};

enum inputs {
    FIRE,
    LEFT,
    RIGHT,
    THRUST,
    START,
    TELEPORT,
    QUIT,
    NUM_INPUTS
};

enum rotate_state {
    ROTATE_LEFT,
    ROTATE_RIGHT,
    ROTATE_STOP
};

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

typedef struct {
    vec2 vel;
    int size;
    float angle;
    float ang_vel;
    vec2 pos;
    enum asteroid_type type;
    enum asteroid_model model;
} asteroid;

typedef struct {
    vec2 pos;
    vec2 vel;
    int active_flag;
    float timer;
    Emitter particles;
} bullet;

typedef struct {
    int state;
    int event;
    int input[NUM_INPUTS];
    float timer;
    int cur_color;
    ship player;
    List *active_asteroids;
    List *inactive_asteroids;
    bullet bullet_list[MAX_BULLETS];
    int num_bullets;
    Bitmap title;
    Bitmap score;
    void (*update)(void *, float);
    void (*render)(void *);
} asteroids;

/******************************************************************************
 * game.c
 * ***************************************************************************/
extern const int COLORS[NUM_COLORS];

int check_for_event(asteroids *game);
int next_state(int current_state, int event);
void get_user_input(asteroids *game, App *app);
void handle_user_input(asteroids *game);
void asteroids_init(asteroids *game);

/******************************************************************************
 * title.c
 * ***************************************************************************/
void title_init(void *game_state);
void title_update(void *game_state, float dt);
void title_render(void *game_state);

/******************************************************************************
 * ship.c
 * ***************************************************************************/
void ship_init(ship *s, float x, float y);
void ship_update(ship *s, float dt);
void ship_render(ship *s);

/******************************************************************************
 * asteroid.c
 * ***************************************************************************/
void load_models();
void asteroid_init(asteroid *a);
void asteroid_update(asteroid *a, float dt);
void asteroid_render(asteroid *a);

/******************************************************************************
 * util.c
 * ***************************************************************************/
vec2 wrap_coor(vec2 pos, int w, int h);

#endif // DEFS_H
