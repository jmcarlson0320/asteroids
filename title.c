#include "defs.h"
#include <stdlib.h>

static void start(asteroids *game)
{
    // center ship
    ship_init(&game->player, WIDTH / 2, HEIGHT / 2);

    clear_asteroids(game);
    for (int i = 0; i < NUM_START_ASTEROIDS; i++) {
        spawn_asteroid(game, rand() % WIDTH * 0.7, rand() % HEIGHT * 0.7, LARGE);
    }

    game->stage = 0;
    game->lives = 3;
    game->score = 0;
    game->enemy_timer = 0;
    transition_to_start(game);
}

static void title_update(asteroids *game, float dt)
{
    if (game->input[FIRE]) {
        start(game);
        return;
    }

    // color change timer
    game->timer += dt;
    if (game->timer >= 0.1f) {
        game->cur_color++;
        game->cur_color %= NUM_COLORS;
        game->timer = 0;
    }

    update_asteroid_list(game->active_asteroids, dt);
}

static void title_render(asteroids *game)
{
    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_render(a);
    }

    draw_text("ASTEROIDS\n", WIDTH / 2 - (7 * 9 / 2), HEIGHT / 2.5, COLORS[game->cur_color]);
    draw_text("press 'FIRE' to start\n", WIDTH / 2 - (7 * 22 / 2), HEIGHT / 2.5 + 18, COLORS[game->cur_color]);
}

void transition_to_title(asteroids *game)
{
    game->update = title_update;
    game->render = title_render;
    game->state = TITLE;

    clear_asteroids(game);
    for (int i = 0; i < 10; i++) {
        asteroid *a = list_pop(game->inactive_asteroids, 0);
        if (a) {
            asteroid_init(a, rand() % 3);
            list_append(game->active_asteroids, a);
        }
    }
}
