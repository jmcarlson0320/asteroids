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
#define BULLET_SPEED 350
#define BULLET_LIFETIME 1
#define NUM_EXPLOSION_PARTICLES 10
#define MAX_EXPLOSIONS 8
#define MAX_SCORE_STRING_LENGTH 11
#define ACTIVE 1
#define INACTIVE 0
#define MAX_ASTEROIDS 64

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
} bullet;

typedef struct {
    vec2 pos;
    vec2 vel;
} explosion_piece;

typedef struct {
    explosion_piece pieces[NUM_EXPLOSION_PARTICLES];
    vec2 pos;
    float lifetime;
    int active_flag;
} explosion;

typedef struct {
    vec2 pos;
    vec2 vel;
    float angle;
    float angular_vel;
} ship_explosion_piece;

typedef struct {
    ship_explosion_piece pieces[NUM_EXPLOSION_PARTICLES];
    vec2 pos;
    float lifetime;
    int active_flag;
} ship_explosion;

typedef struct gamestate gamestate;
typedef struct asteroids asteroids;

//function pointers for events
typedef void (*on_gameover)(asteroids *game);
typedef void (*on_start)(asteroids *game);
typedef void (*on_timer)(asteroids *game);
typedef void (*on_destroyed)(asteroids *game);
typedef void (*on_initials_entered)(asteroids *game);
typedef void (*on_level_cleared)(asteroids *game);

// each state implements versions of these
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
    int score;
    int lives;
    float enemy_timer;
    ship player;
    List *active_asteroids;
    List *inactive_asteroids;
    bullet bullet_list[MAX_BULLETS];
    int num_bullets;
    explosion explosion_list[MAX_EXPLOSIONS];
    ship_explosion ship_explosion;
    Bitmap title;
    Bitmap score_board;
};

/******************************************************************************
 * game.c
 * ***************************************************************************/
extern const int COLORS[NUM_COLORS];

void get_user_input(asteroids *game, App *app);
void handle_user_input(asteroids *game);
void asteroids_init(asteroids *game);

void default_state(gamestate *gamestate);

void clear_asteroids(asteroids *game);
int spawn_asteroid(asteroids *game, float x, float y, enum asteroid_type type);
explosion *find_inactive_explosion(explosion *expl_array, int size);

// state transitions
void transition_to_title(asteroids *game);
void transition_to_start(asteroids *game);
void transition_to_reset(asteroids *game);
void transition_to_play(asteroids *game);
void transition_to_gameover(asteroids *game);
void transition_to_test(asteroids *game);

// events
void asteroids_update(asteroids *game, float dt);
void asteroids_render(asteroids *game);
void gameover_event(asteroids *game);
void start_event(asteroids *game);
void timer_event(asteroids *game);
void destroyed_event(asteroids *game);
void initials_entered_event(asteroids *game);
void level_cleared_event(asteroids *game);

/******************************************************************************
 * ship.c
 * ***************************************************************************/
void ship_init(ship *s, float x, float y);
void ship_update(ship *s, float dt);
void ship_render(ship *s);
void draw_ship_wireframe(int x, int y);

/******************************************************************************
 * asteroid.c
 * ***************************************************************************/
extern const int asteroid_scales[NUM_TYPES];
extern const int asteroid_scores[NUM_TYPES];

void load_models();
void asteroid_init(asteroid *a, enum asteroid_type type);
void asteroid_update(asteroid *a, float dt);
void asteroid_render(asteroid *a);

/******************************************************************************
 * explosion.c
 * ***************************************************************************/
void explosion_init(explosion *e);
void explosion_start(explosion *e, int x, int y);
void explosion_update(explosion *e, float dt);
void explosion_render(explosion *e);

/******************************************************************************
 * ship_explosion.c
 * ***************************************************************************/
void ship_explosion_init(ship_explosion *e);
void ship_explosion_start(ship_explosion *e, int x, int y);
void ship_explosion_update(ship_explosion *e, float dt);
void ship_explosion_render(ship_explosion *e);

/******************************************************************************
 * util.c
 * ***************************************************************************/
vec2 wrap_coor(vec2 pos, int w, int h);
void clear_screen();
float dist(vec2 *u, vec2 *v);
int point_in_circle(vec2 *point, vec2 *origin, float radius);
int circle_overlap(vec2 *origin_a, float radius_a, vec2 *origin_b, float radius_b);
float rand_float(float min, float max);

#endif // DEFS_H
