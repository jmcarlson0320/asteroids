#include "defs.h"
#include <stdio.h>

#define PAUSE_LENGTH 1.5f

static float timer_sec = 0;
static float timer_display = 0;
static int display_flag = ACTIVE;

static void timer(asteroids *game)
{
    transition_to_play(game);
}

static void reset_update(asteroids *game, float dt)
{
    timer_sec += dt;
    timer_display += dt;
    if (timer_sec >= PAUSE_LENGTH) {
        timer(game);
    }

    if (timer_display >= 0.2f) {
        timer_display = 0.0f;
        display_flag = !display_flag;
    }

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

    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosion *e = &game->explosion_list[i];
        if (e->active_flag) {
            explosion_update(e, dt);
        }
    }

}

static void reset_render(asteroids *game)
{
    clear_screen();
    if (display_flag) { // flash the ship
        ship_render(&game->player);
    }

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

    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosion *e = &game->explosion_list[i];
        if (e->active_flag) {
            explosion_render(e);
        }
    }

    char score_string[MAX_SCORE_STRING_LENGTH];
    snprintf(score_string, MAX_SCORE_STRING_LENGTH, "%d", game->score);
    draw_text(score_string, 8, 5, 0xffffff);

    int x_offset = 8;
    int y_offset = 30;
    int spacing = 15;
    for (int i = 0; i < game->lives; i++) {
        draw_ship_wireframe(x_offset + i * spacing, y_offset);
    }
}

void transition_to_reset(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.timer = timer;
    game->gamestate.update = reset_update;
    game->gamestate.render= reset_render;

    timer_sec = 0.0f;
    timer_display = 0.0f;
    display_flag = ACTIVE;
}
