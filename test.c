#include "defs.h"

static ship_explosion boom;

static void test_update(asteroids *game, float dt)
{
    if (game->input[FIRE]) {
        ship_explosion_start(&boom, WIDTH / 2, HEIGHT / 2);
    }

    ship_explosion_update(&boom, dt);
}

static void test_render(asteroids *game)
{
    clear_screen();
    ship_explosion_render(&boom);
}

void transition_to_test(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.update = test_update;
    game->gamestate.render = test_render;
    ship_explosion_init(&boom);
}
