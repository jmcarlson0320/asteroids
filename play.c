#include "defs.h"
#include <stdlib.h>

static void destroyed(asteroids *game)
{
    transition_to_reset(game);
}

static void gameover(asteroids *game)
{
    transition_to_gameover(game);
}

static void cleared(asteroids *game)
{
    transition_to_reset(game);
}

static void play_update(asteroids *game, float dt)
{
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
                vec2_mult(&b->vel, &b->vel, BULLET_SPEED);
                vec2_add(&b->vel, &b->vel, &s->vel);
                game->num_bullets++;
                break;
            }
        }
    }

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
            }
        }
    }

    check_collisions(game);
}

static void play_render(asteroids *game)
{
    clear_screen();
    ship_render(&game->player);

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_render(a);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet b = game->bullet_list[i];
        if (b.active_flag) {
            draw_fill_circle(b.pos.e[X_COOR], b.pos.e[Y_COOR], 1, 0xffffff);
        }
    }
}

void transition_to_play(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.destroyed = destroyed;
    game->gamestate.gameover = gameover;
    game->gamestate.cleared = cleared;
    game->gamestate.update = play_update;
    game->gamestate.render = play_render;

    // center ship
    ship_init(&game->player, WIDTH / 2, HEIGHT / 2);

    // two large asteroids
    clear_asteroids(game);
    spawn_asteroid(game, rand() % WIDTH * 0.7, rand() % HEIGHT * 0.7, LARGE);
    spawn_asteroid(game, rand() % WIDTH * 0.7, rand() % HEIGHT * 0.7, LARGE);

    // zero out score
    game->score = 0;

    // reset enemy timer
    game->enemy_timer = 0;
}
