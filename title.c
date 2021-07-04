#include "defs.h"

void title_init(void *game_state)
{
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
