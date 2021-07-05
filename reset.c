#include "defs.h"

static void timer(asteroids *game)
{
    transition_to_play(game);
}

static void reset_update(asteroids *game, float dt)
{
}

static void reset_render(asteroids *game)
{
    clear_screen();
    draw_text("reset state", 0, 0, 0x00ffff);
}

void transition_to_reset(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.timer = timer;
    game->gamestate.update = reset_update;
    game->gamestate.render= reset_render;
}

