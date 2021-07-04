#include <tiny-fw.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list.h"
#include "particle.h"
#include "defs.h"

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

#define BULLET_LIFETIME 1
typedef struct {
    vec2 pos;
    vec2 vel;
    int active_flag;
    float timer;
    Emitter particles;
} bullet;

/******************************************************************************
 * game state
 * ***************************************************************************/
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

#define MAX_BULLETS 4

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

int check_for_event(asteroids *game)
{
    return NO_EVENT;
}
int next_state(int current_state, int event)
{
    if (event == EXIT) {
        return SHUTDOWN;
    }

    int next = current_state;
    switch (current_state) {
        case TITLE:
            if (event == START) {
                next = RESET;
            }
            break;
        case PLAY:
            if (event == DESTROYED) {
                next = RESET;
            } else if (event == LEVEL_CLEARED) {
                next = RESET;
            } else if (event == DEFEATED) {
                next = GAME_OVER;
            }
            break;
        case RESET:
            if (event == TIMER) {
                next = PLAY;
            }
            break;
        case GAME_OVER:
            if (event == INITIALS_ENTERED) {
                next = TITLE;
            }
            break;
        default:
            break;
    }

    return next;
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

void get_user_input(asteroids *game, App *app)
{
    game->input[FIRE] = app->keyboard.pressed[KEY_MAP[FIRE]];

    // take keyboard state from app and update game's controls
    for (int i = 1; i < NUM_INPUTS; i++) {
        game->input[i] = app->keyboard.down[KEY_MAP[i]];
    }

    if (game->input[QUIT] == ACTIVE) {
        app->running = 0;
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

void handle_user_input(asteroids *game)
{
    // ship rotation
    ship *s = &game->player;
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

    // bullets
    if (game->input[FIRE] && game->num_bullets < MAX_BULLETS) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullet *b = &game->bullet_list[i];
            if (!b->active_flag) {
                b->active_flag = ACTIVE;
                b->timer = 0;
                b->pos = s->pos;
                b->vel = vec2_unit_vec(s->angle);
                emitter_reset_particles(&b->particles);
                b->particles.pos = b->pos;
                vec2_mult(&b->vel, &b->vel, 500);
                vec2_add(&b->vel, &b->vel, &s->vel);
                game->num_bullets++;
                break;
            }
        }
    }
}

void title_init(void *game_state)
{
    asteroids *game = (asteroids *) game_state;
}

void title_update(void *game_state, float dt)
{
    asteroids *game = (asteroids *) game_state;
    handle_user_input(game);

    // color change timer
    game->timer += dt;
    if (game->timer >= 0.1f) {
        game->cur_color++;
        game->cur_color %= NUM_COLORS;
        game->timer = 0;
    }

    ship_update(&game->player, dt);

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_update(a, dt);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet *b = &game->bullet_list[i];
        if (b->active_flag) {
            if (b->timer > BULLET_LIFETIME) {
                b->active_flag = INACTIVE;
                game->num_bullets--;
            } else {
                vec2 ds;
                vec2_mult(&ds, &b->vel, dt);
                vec2_add(&b->pos, &b->pos, &ds);
                b->timer += dt;
                b->particles.pos = b->pos;
                emitter_update(&b->particles, dt);
            }
        }
    }

}

void title_render(void *game_state)
{
    asteroids *game = (asteroids *) game_state;
    draw_fill_rect(0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
    ship_render(&game->player);

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_render(a);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet b = game->bullet_list[i];
        if (b.active_flag) {
            draw_point(b.pos.e[X_COOR], b.pos.e[Y_COOR], 0xffffff);
            emitter_render(&b.particles);
        }
    }

    draw_text("asteroids test!!!\n"
              "press 'Q' to quit\n"
              "press 'N' to add an asteroid\n"
              "press 'M' to remove an asteroid\n"
              "press 'SPACE' to fire\n"
              "press 'WASD' to move",
              0, 0, COLORS[game->cur_color]);
}

void play_init(void *game_state)
{
    asteroids *game = (asteroids *) game_state;
}

void play_update(void *game_state, float dt)
{
    asteroids *game = (asteroids *) game_state;
}

void play_render(void *game_state)
{
    asteroids *game = (asteroids *) game_state;
}

void asteroids_init(asteroids *game)
{
    game->state = TITLE;
    game->event = NO_EVENT;

    for (int i = 0; i < NUM_INPUTS; i++) {
        game->input[i] = 0;
    }

    srand(time(NULL));
    game->timer = 0.0f;

    game->cur_color = RED;

    ship_init(&game->player, WIDTH / 2, HEIGHT / 2);

    load_models();
    game->active_asteroids = list_new();
    game->inactive_asteroids = list_new();
    for (int i = 0; i < 10; i++) {
        asteroid *a = malloc(sizeof(asteroid));
        asteroid_init(a);
        list_append(game->inactive_asteroids, a);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet *b = &game->bullet_list[i];
        b->pos = new_vec2(0, 0);
        b->vel = new_vec2(0, 0);
        b->active_flag = INACTIVE;
        b->timer = 0;
        b->particles = emitter_create(200, b->pos.e[X_COOR], b->pos.e[Y_COOR]);
        emitter_reset_particles(&b->particles);
    }
    game->num_bullets = 0;

    game->update = title_update;
    game->render = title_render;
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
        app_update(&app);
        get_user_input(&game, &app);

        // state transition
        game.event = check_for_event(&game);
        if (game.event != NO_EVENT) {
            game.state = next_state(game.state, game.event);
            game.event = NO_EVENT;
        }

        // the function pointers point to the update/render functions of the current gamestate
        (*game.update)(&game, app.time.dt_sec);
        (*game.render)(&game);

        app_draw_graphics(&app);
    }
    app_quit(&app);

    return 0;
}
