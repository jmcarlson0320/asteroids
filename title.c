#include "defs.h"
#include <stdlib.h>

static void start(asteroids *game)
{
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
    transition_to_start(game);
}

static void title_update(asteroids *game, float dt)
{
    if (game->input[FIRE]) {
        start(game);
    }
    // color change timer
    game->timer += dt;
    if (game->timer >= 0.1f) {
        game->cur_color++;
        game->cur_color %= NUM_COLORS;
        game->timer = 0;
    }

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_update(a, dt);
    }
}

static void title_render(asteroids *game)
{
    clear_screen();

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
    default_state(&game->gamestate);
    game->gamestate.start = start;
    game->gamestate.update = title_update;
    game->gamestate.render = title_render;

    clear_asteroids(game);
    for (int i = 0; i < 10; i++) {
        asteroid *a = list_pop(game->inactive_asteroids, 0);
        if (a) {
            asteroid_init(a, rand() % 3);
            list_append(game->active_asteroids, a);
        }
    }
}
