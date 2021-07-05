#include "defs.h"

static void timer(gamestate *gamestate)
{
    transition_to_play(gamestate);
}

static void reset_update(asteroids *game, float dt)
{
}

static void reset_render(asteroids *game)
{
    clear_screen();
    draw_text("reset state", 0, 0, 0x00ffff);
}

void transition_to_reset(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->timer = timer;
    gamestate->update = reset_update;
    gamestate->render= reset_render;
}

