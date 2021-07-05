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
    TEST,
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

typedef struct gamestate gamestate;
typedef struct asteroids asteroids;

//function pointers for events
typedef void (*on_gameover)(gamestate *gamestate);
typedef void (*on_start)(gamestate *gamestate);
typedef void (*on_timer)(gamestate *gamestate);
typedef void (*on_destroyed)(gamestate *gamestate);
typedef void (*on_initials_entered)(gamestate *gamestate);
typedef void (*on_level_cleared)(gamestate *gamestate);

//function pointers for update and render
typedef void (*on_update)(asteroids *game, float dt);
typedef void (*on_render)(asteroids *game);

struct gamestate {
    on_update update;
    on_render render;
    on_gameover gameover;
    on_start start;
    on_timer timer;
    on_destroyed destroyed;
    on_initials_entered initials_entered;
    on_level_cleared cleared;
};

struct asteroids {
    gamestate gamestate;
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
};

/******************************************************************************
 * game.c
 * ***************************************************************************/
extern const int COLORS[NUM_COLORS];
void default_state(gamestate *gamestate);

void get_user_input(asteroids *game, App *app);
void handle_user_input(asteroids *game);
void asteroids_init(asteroids *game);
void default_state(gamestate *gamestate);

void asteroids_update(asteroids *game, float dt);
void asteroids_render(asteroids *game);

void gameover_event(asteroids *game);
void start_event(asteroids *game);
void timer_event(asteroids *game);
void destroyed_event(asteroids *game);
void initials_entered_event(asteroids *game);
void level_cleared_event(asteroids *game);

/******************************************************************************
 * test.c
 * ***************************************************************************/
void test_init(void *game_state);
void test_update(void *game_state, float dt);
void test_render(void *game_state);

/******************************************************************************
 * title.c
 * ***************************************************************************/
void transition_to_title(gamestate *gamestate);
void title_init(void *game_state);
void title_update(void *game_state, float dt);
void title_render(void *game_state);

/******************************************************************************
 * reset.c
 * ***************************************************************************/
void transition_to_reset(gamestate *gamestate);
void reset_init(void *game_state);
void reset_update(void *game_state, float dt);
void reset_render(void *game_state);

/******************************************************************************
 * play.c
 * ***************************************************************************/
void transition_to_play(gamestate *gamestate);
void play_init(void *game_state);
void play_update(void *game_state, float dt);
void play_render(void *game_state);

/******************************************************************************
 * gameover.c
 * ***************************************************************************/
void transition_to_gameover(gamestate *gamestate);
void gameover_init(void *game_state);
void gameover_update(void *game_state, float dt);
void gameover_render(void *game_state);

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
void clear_screen();

#endif // DEFS_H
