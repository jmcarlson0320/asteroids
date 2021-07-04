#include "defs.h"
#include <stdlib.h>
#include <time.h>

const int COLORS[NUM_COLORS] = {
    [RED] = 0xff0000,
    [ORANGE] = 0xff7f00,
    [YELLOW] = 0xffff00,
    [GREEN] = 0x00ff00,
    [BLUE] = 0x0000ff,
    [INDIGO] = 0x4b0082,
    [VIOLET] = 0x7f00ff
};

const int KEY_MAP[NUM_INPUTS] = {
    [FIRE] = KEY_SPACE,
    [LEFT] = KEY_A,
    [RIGHT] = KEY_D,
    [THRUST] = KEY_W,
    [START] = KEY_RETURN,
    [TELEPORT] = KEY_T,
    [QUIT] = KEY_Q
};

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
