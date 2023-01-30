#include "defs.h"

static void test_update(asteroids *game, float dt)
{
}

static void test_render(asteroids *game)
{
}

void transition_to_test(asteroids *game)
{
    game->update = test_update;
    game->render = test_render;
    game->state = TEST;
}
