#include <tiny-fw.h>
#include <math.h>
#include "list.h"
#include <stdlib.h>
#include <time.h>

#define SCALE_FACTOR 3
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)

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

const int COLORS[NUM_COLORS] = {
    [RED] = 0xff0000,
    [ORANGE] = 0xff7f00,
    [YELLOW] = 0xffff00,
    [GREEN] = 0x00ff00,
    [BLUE] = 0x0000ff,
    [INDIGO] = 0x4b0082,
    [VIOLET] = 0x7f00ff
};

/******************************************************************************
 * asteroid
 * ***************************************************************************/
enum asteroid_type {
    SMALL,
    MED,
    LARGE
};

#define NUM_POINTS_ASTEROID 10
vec2 large_asteroid[NUM_POINTS_ASTEROID] = {
    {{1, 2}},
    {{3, 3}},
    {{4, 2}},
    {{2, 0}},
    {{3, -1}},
    {{1, -3}},
    {{-2, -3}},
    {{-3, -1}},
    {{-3, 2}},
    {{-1, 3}}
};

typedef struct {
    vec2 vel;
    int size;
    float angle;
    float ang_vel;
    vec2 pos;
    enum asteroid_type type;
} asteroid;

void asteroid_init(asteroid *a)
{
    a->vel = new_vec2((float) rand() / (float) RAND_MAX * 100.0f - 50.0f, (float) rand() / (float) RAND_MAX * 100.0f - 50.0f);
    a->size = 10;
    a->angle = (float) rand() / (float) RAND_MAX * 2.0f * M_PI;
    a->ang_vel = (float) rand() / (float) RAND_MAX * 0.02f - 0.01f;
    a->pos = new_vec2((float) rand() / (float) RAND_MAX * WIDTH, (float) rand() / (float) RAND_MAX * HEIGHT);
    a->type = LARGE;
}

void asteroid_update(asteroid *a, float dt)
{
    a->angle += a->ang_vel;

    vec2 ds;
    vec2_mult(&ds, &a->vel, dt);
    vec2_add(&a->pos, &a->pos, &ds);

    if (a->pos.e[X_COOR] < -a->size) {
        a->pos.e[X_COOR] = WIDTH + a->pos.e[X_COOR];
    } else if (a->pos.e[X_COOR] >= WIDTH + a->size) {
        a->pos.e[X_COOR] = a->pos.e[X_COOR] - WIDTH - a->size;
    } else if (a->pos.e[Y_COOR] < -a->size) {
        a->pos.e[Y_COOR] = HEIGHT + a->pos.e[Y_COOR];
    } else if (a->pos.e[Y_COOR] >= HEIGHT + a->size) {
        a->pos.e[Y_COOR] = a->pos.e[Y_COOR] - HEIGHT - a->size;
    }
}

void asteroid_render(asteroid *a)
{
    // build transformation, needs to be in this order
    transform t = new_transform();
    transform_scale(&t, a->size);
    transform_rotate(&t, a->angle);
    transform_translate(&t, a->pos.e[X_COOR], a->pos.e[Y_COOR]);
    draw_wireframe(large_asteroid, NUM_POINTS_ASTEROID, &t);
}


/******************************************************************************
 * ship
 * ***************************************************************************/
enum rotate_state {
    ROTATE_LEFT,
    ROTATE_RIGHT,
    ROTATE_STOP
};

vec2 ship_model[3] = {
    {{ 0.5f,  0.0f}},
    {{-0.5f,  0.25f}},
    {{-0.5f, -0.25f}}
};

typedef struct {
    vec2 *model;
    float scale;
    float angle;
    vec2 pos;
    vec2 vel;
    float drag;

    enum rotate_state ctl_rotate;
    int ctl_thrust;
} ship;

void ship_init(ship *s, vec2 *model)
{
    s->model = model;
    s->angle = 3.0f * M_PI / 2.0f;
    s->scale = 15;
    s->pos = new_vec2(WIDTH / 2, HEIGHT / 2);

    s->vel = new_vec2(0, 0);
    s->drag = 0.99f;

    s->ctl_rotate = ROTATE_STOP;
    s->ctl_thrust = 0;
}

void ship_update(ship *s, float dt)
{
    switch (s->ctl_rotate) {
        case ROTATE_LEFT:
            s->angle -= 0.05;
            break;
        case ROTATE_RIGHT:
            s->angle += 0.05;
            break;
        default:
            break;
    }

    if (s->ctl_thrust) {
        vec2 dv = new_vec2(cos(s->angle), sin(s->angle));
        vec2_normalize(&dv, &dv);
        vec2_mult(&dv, &dv, 3);
        vec2_add(&s->vel, &s->vel, &dv);
    }

    vec2_mult(&s->vel, &s->vel, s->drag);

    vec2 ds;
    vec2_mult(&ds, &s->vel, dt);
    vec2_add(&s->pos, &s->pos, &ds);
}

void ship_render(ship *s)
{
    // build transformation, needs to be in this order
    transform t = new_transform();
    transform_scale(&t, s->scale);
    transform_rotate(&t, s->angle);
    transform_translate(&t, s->pos.e[X_COOR], s->pos.e[Y_COOR]);
    draw_wireframe(ship_model, 3, &t);
}

typedef struct {
    vec2 pos;
    vec2 vel;
    int active_flag;
} bullet;

/******************************************************************************
 * game state
 * ***************************************************************************/
enum game_states {
    TITLE,
    PLAY,
    RESET,
    GAME_OVER,
    NUM_STATES
};

enum game_events {
    START_GAME,
    DESTROYED,
    TIMER,
    LEVEL_CLEARED,
    DEFEATED,
    EXIT,
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

#define MAX_BULLETS 4

typedef struct {
    int state;
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
} asteroids;

int next_state(int current_state, int event)
{
    return 0;
}

int next_event(asteroids *game)
{
    return 0;
}

#define ACTIVE 1
#define INACTIVE 0

const int KEY_MAP[NUM_INPUTS] = {
    [FIRE] = KEY_SPACE,
    [LEFT] = KEY_A,
    [RIGHT] = KEY_D,
    [THRUST] = KEY_W,
    [START] = KEY_RETURN,
    [TELEPORT] = KEY_T,
    [QUIT] = KEY_Q
};

void process_input(asteroids *game, App *app)
{
    game->input[FIRE] = app->keyboard.pressed[KEY_MAP[FIRE]];

    // take keyboard state from app and update game's controls
    for (int i = 1; i < NUM_INPUTS; i++) {
        game->input[i] = app->keyboard.down[KEY_MAP[i]];
    }

    if (game->input[QUIT] == ACTIVE) {
        app->running = 0;
    }

    ship *s = &game->player;

    // ship rotation
    if (game->input[LEFT] && game->input[RIGHT]) {
        s->ctl_rotate = ROTATE_STOP;
    } else if (game->input[LEFT]) {
        s->ctl_rotate = ROTATE_LEFT;
    } else if (game->input[RIGHT]) {
        s->ctl_rotate = ROTATE_RIGHT;
    } else {
        s->ctl_rotate = ROTATE_STOP;
    }

    // ship thrust
    if (game->input[THRUST]) {
        s->ctl_thrust = 1;
    } else {
        s->ctl_thrust = 0;
    }

    if (app->keyboard.pressed[KEY_MAP[FIRE]] && game->num_bullets < MAX_BULLETS) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullet *b = &game->bullet_list[i];
            if (!b->active_flag) {
                b->active_flag = ACTIVE;
                b->pos = s->pos;
                b->vel = vec2_unit_vec(s->angle);
                vec2_mult(&b->vel, &b->vel, 75);
                game->num_bullets++;
                break;
            }
        }
    }

    if (app->keyboard.pressed[KEY_N]) {
        asteroid *a = list_pop(game->inactive_asteroids, 0);
        if (a) {
            asteroid_init(a);
            list_append(game->active_asteroids, a);
        }
    }

    if (app->keyboard.pressed[KEY_M]) {
        asteroid *a = list_pop(game->active_asteroids, 0);
        if (a) {
            list_append(game->inactive_asteroids, a);
        }
    }
}

void asteroids_init(asteroids *game)
{
    srand(time(NULL));
    game->timer = 0.0f;
    game->cur_color = RED;

    for (int i = 0; i < NUM_INPUTS; i++) {
        game->input[i] = 0;
    }

    ship_init(&game->player, ship_model);

    game->active_asteroids = list_new();
    game->inactive_asteroids = list_new();
    for (int i = 0; i < 10; i++) {
        asteroid *a = malloc(sizeof(asteroid));
        asteroid_init(a);
        list_append(game->inactive_asteroids, a);
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
        game->bullet_list[i].pos = new_vec2(0, 0);
        game->bullet_list[i].vel = new_vec2(0, 0);
        game->bullet_list[i].active_flag = 0;
    }
    game->num_bullets = 0;
}

void asteroids_update(asteroids *game, App *app)
{
    process_input(game, app);

    // color change timer
    game->timer += app->time.dt_sec;
    if (game->timer >= 0.1f) {
        game->cur_color++;
        game->cur_color %= NUM_COLORS;
        game->timer = 0;
    }

    ship_update(&game->player, app->time.dt_sec);

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_update(a, app->time.dt_sec);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet *b = &game->bullet_list[i];
        if (b->active_flag) {
            vec2 ds;
            vec2_mult(&ds, &b->vel, app->time.dt_sec);
            vec2_add(&b->pos, &b->pos, &ds);
        }
    }

    // state transition
    game->state = next_state(game->state, next_event(game));

}

void asteroids_render(asteroids *game)
{
    draw_fill_rect(0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
    ship_render(&game->player);
    draw_text("asteroids!!!\n"
              "press 'Q' to quit",
              0, 0, COLORS[game->cur_color]);

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_render(a);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet b = game->bullet_list[i];
        if (b.active_flag) {
            draw_point(b.pos.e[X_COOR], b.pos.e[Y_COOR], 0xffffff);
        }
    }
}

/******************************************************************************
 * main loop
 * ***************************************************************************/
int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT);
    asteroids game;
    asteroids_init(&game);
    app_start(&app);
    while (app.running) {
        app_begin_frame(&app);
        asteroids_update(&game, &app);
        asteroids_render(&game);
        app_end_frame(&app);
    }
    app_quit(&app);

    return 0;
}
