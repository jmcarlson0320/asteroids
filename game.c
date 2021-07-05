#include "defs.h"
#include <stdio.h>
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

static void default_state_transition(asteroids *game)
{
    printf("state transition not defined!!!\n");
}

static void default_update(asteroids *game, float dt)
{
    printf("update function not defined!!!\n");
}

static void default_render(asteroids *game)
{
    printf("render function not defined!!!\n");
}

void default_state(gamestate *gamestate)
{
    gamestate->update = default_update;
    gamestate->render = default_render;
    gamestate->gameover = default_state_transition;
    gamestate->start = default_state_transition;
    gamestate->timer = default_state_transition;
    gamestate->destroyed = default_state_transition;
    gamestate->initials_entered = default_state_transition;
    gamestate->cleared = default_state_transition;
}

void asteroids_update(asteroids *game, float dt)
{
    game->gamestate.update(game, dt);
}

void asteroids_render(asteroids *game)
{
    game->gamestate.render(game);
}

void gameover_event(asteroids *game)
{
    game->gamestate.gameover(game);
}

void start_event(asteroids *game)
{
    game->gamestate.start(game);
}

void timer_event(asteroids *game)
{
    game->gamestate.timer(game);
}

void destroyed_event(asteroids *game)
{
    game->gamestate.destroyed(game);
}

void initials_entered_event(asteroids *game)
{
    game->gamestate.initials_entered(game);
}

void level_cleared_event(asteroids *game)
{
    game->gamestate.cleared(game);
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

    if (app->keyboard.pressed[KEY_T]) {
        transition_to_title(game);
    }

    if (app->keyboard.pressed[KEY_P]) {
        transition_to_play(game);
    }

    if (app->keyboard.pressed[KEY_G]) {
        transition_to_gameover(game);
    }

    if (app->keyboard.pressed[KEY_R]) {
        transition_to_reset(game);
    }

    if (app->keyboard.pressed[KEY_D]) {
        gameover_event(game);
    }

    if (app->keyboard.pressed[KEY_S]) {
        start_event(game);
    }

    if (app->keyboard.pressed[KEY_I]) {
        timer_event(game);
    }

    if (app->keyboard.pressed[KEY_K]) {
        destroyed_event(game);
    }

    if (app->keyboard.pressed[KEY_L]) {
        initials_entered_event(game);
    }

    if (app->keyboard.pressed[KEY_C]) {
        level_cleared_event(game);
    }
}

// separate this into update functions
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
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
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

    transition_to_title(game);
}
