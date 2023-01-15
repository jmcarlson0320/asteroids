#include "defs.h"
#include <stdio.h>

#define EXPLODE_TIME 1.5f
#define RESET_TIME 3.2f

enum phase {
    EXPLODE,
    FLASH_SHIP
};

static int phase = EXPLODE;
static float reset_timer = 0;
static float flash_timer = 0;
static int display_flag = ACTIVE;

static void timer(asteroids *game)
{
    if (game->lives < 0) {
        transition_to_gameover(game);
    } else {
        transition_to_play(game);
    }
}

static void reset_update(asteroids *game, float dt)
{
    // timer ticks
    reset_timer += dt;
    flash_timer += dt;
    if (reset_timer >= EXPLODE_TIME) {
        if (game->lives < 0) {
            transition_to_gameover(game);
        }
        phase = FLASH_SHIP;

    }
    if (reset_timer >= RESET_TIME) {
        timer(game);
    }

    if (flash_timer >= 0.2f) {
        flash_timer = 0.0f;
        display_flag = !display_flag;
    }

    update_asteroid_list(game->active_asteroids, dt);
    update_bullets(&game->bullet_list, dt);
    update_explosions(game->explosion_list, dt);
    ship_explosion_update(&game->ship_explosion, dt);
}

static void reset_render(asteroids *game)
{
    if (phase == EXPLODE) {
        ship_explosion_render(&game->ship_explosion);
    } else if (phase == FLASH_SHIP) {
        if (display_flag) { // flash the ship
            ship_render(&game->player);
        }
    }

    // render asteroids
    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_render(a);
    }

    // render bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet b = game->bullet_list.bullets[i];
        if (b.active_flag) {
            draw_fill_circle(b.pos.e[X_COOR], b.pos.e[Y_COOR], 1, 0xffffff);
        }
    }

    // render explosions
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosion *e = &game->explosion_list[i];
        if (e->active_flag) {
            explosion_render(e);
        }
    }

    // render score
    char score_string[MAX_SCORE_STRING_LENGTH];
    snprintf(score_string, MAX_SCORE_STRING_LENGTH, "%d", game->score);
    draw_text(score_string, 8, 5, 0xffffff);

    // render lives
    int x_offset = 8;
    int y_offset = 30;
    int spacing = 15;
    for (int i = 0; i < game->lives; i++) {
        draw_ship_wireframe(x_offset + i * spacing, y_offset);
    }
}

void transition_to_reset(asteroids *game)
{
    game->update = reset_update;
    game->render= reset_render;

    reset_timer = 0.0f;
    flash_timer = 0.0f;
    display_flag = INACTIVE;
    phase = EXPLODE;
    ship_explosion_start(&game->ship_explosion, game->player.pos.e[X_COOR], game->player.pos.e[Y_COOR]);
}
