#include "defs.h"

// start game event
static void start(gamestate *gamestate)
{
    transition_to_reset(gamestate);
}

static void title_update(asteroids *game, float dt)
{
}

static void title_render(asteroids *game)
{
    clear_screen();
    draw_text("title state", 0, 0, 0x0000ff);
}

void transition_to_title(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->start = start;
    gamestate->update = title_update;
    gamestate->render = title_render;
}
