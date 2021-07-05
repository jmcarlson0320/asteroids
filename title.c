#include "defs.h"

// start game event
static void start(gamestate *gamestate)
{
    transition_to_reset(gamestate);
}

// any state --> title
void transition_to_title(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->start = start;
}

void title_init(void *game_state)
{
}

void title_update(void *game_state, float dt)
{
}

void title_render(void *game_state)
{
    draw_text("title state", 0, 0, 0x0000ff);
}
