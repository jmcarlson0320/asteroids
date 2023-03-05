#ifndef DEFS_H
#define DEFS_H

#include <tiny-fw.h>
#include "particle.h"
#include "list.h"

#define SCALE_FACTOR 4
#define WIDTH 400
#define HEIGHT 300
#define NUM_POINTS_ASTEROID 11
#define MAX_BULLETS 4
#define BULLET_SPEED 350
#define BULLET_LIFETIME 1
#define ENEMY_SPEED 100
#define NUM_EXPLOSION_PARTICLES 10
#define NUM_SHIP_PIECES 6
#define MAX_EXPLOSIONS 8
#define MAX_SCORE_STRING_LENGTH 11
#define ACTIVE 1
#define INACTIVE 0
#define MAX_ASTEROIDS 64
#define NUM_START_ASTEROIDS 2
#define NUM_HIGHSCORES 10

enum gamestate {
    TITLE,
    START,
    PLAY,
    RESET,
    GAMEOVER,
    HIGHSCORE,
    TEST,
    NUM_GAMESTATES
};

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

enum inputs {
    FIRE,
    LEFT,
    RIGHT,
    THRUST,
    START_GAME,
    TELEPORT,
    QUIT,
    NEXT_LETTER,
    PREV_LETTER,
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
    vec2 pos;
    vec2 vel;
    float angle;
    float scale;
    float shot_timer;
    vec2 *model;
    int active;
} enemy;

typedef struct {
    vec2 pos;
    vec2 vel;
    float angle;
    float ang_vel;
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
    bullet bullets[MAX_BULLETS];
    int num_active_bullets;
} bullet_list;

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

typedef struct {
    int points;
    char initials[4];
} score;

typedef struct asteroids asteroids;

struct asteroids {
    void (*update)(asteroids *game, float dt);
    void (*render)(asteroids *game);
    int state;
    int input[NUM_INPUTS];
    float timer;
    int cur_color;
    int score;
    int lives;
    int stage;
    float enemy_timer;
    ship player;
    enemy enemy;
    List *active_asteroids;
    List *inactive_asteroids;
    bullet_list bullet_list;
    bullet_list enemy_bullets;
    explosion explosion_list[MAX_EXPLOSIONS];
    ship_explosion ship_explosion;
    score score_board[NUM_HIGHSCORES];
    Bitmap title;
};

/******************************************************************************
 * game.c
 * ***************************************************************************/
extern const int COLORS[NUM_COLORS];
extern const int KEY_MAP[NUM_INPUTS];

void asteroids_init(asteroids *game);
void get_user_input(asteroids *game, App *app);
void asteroids_update(asteroids *game, float dt);
void asteroids_render(asteroids *game);
void asteroids_shutdown(asteroids *game);

void clear_asteroids(asteroids *game);
int spawn_asteroid(asteroids *game, float x, float y, enum asteroid_type type);
void update_asteroid_list(List *asteroids, float dt);
void init_bullets(bullet_list *bullet_list);
void update_bullets(bullet_list *bullet_list, float dt);
void render_bullets(bullet_list *bullet_list);
bullet *find_inactive_bullet(bullet_list *bullet_list);
void fire_bullet(bullet_list *bullet_list, vec2 origin, vec2 origin_vel, float angle);
explosion *find_inactive_explosion(explosion *expl_array, int size);
void update_explosions(explosion *expl_array, float dt);
void explode_asteroid(asteroids *game, asteroid *a);
void load_scores(asteroids *game, char *filename);
void save_scores(asteroids *game, char *filename);

void transition_to_title(asteroids *game);
void transition_to_start(asteroids *game);
void transition_to_reset(asteroids *game);
void transition_to_play(asteroids *game);
void transition_to_gameover(asteroids *game);
void transition_to_test(asteroids *game);
void transition_to_highscore(asteroids *game);

/******************************************************************************
 * ship.c
 * ***************************************************************************/
void load_ship_model();
void free_ship_model();
void ship_init(ship *s, float x, float y);
void ship_update(ship *s, float dt);
void ship_render(ship *s);
void draw_ship_wireframe(int x, int y);

/******************************************************************************
 * enemy.c
 * ***************************************************************************/
void load_enemy_model();
void free_enemy_model();
void enemy_init(enemy *e);
void enemy_update(enemy *e, float dt);
void enemy_render(enemy *e);
void spawn_enemy(enemy *e);
void target_player_position(vec2 player_position);

/******************************************************************************
 * asteroid.c
 * ***************************************************************************/
extern const int asteroid_scales[NUM_TYPES];
extern const int asteroid_scores[NUM_TYPES];

void asteroid_load_models();
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
