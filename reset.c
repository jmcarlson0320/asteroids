#include "defs.h"

// timer event
static void timer(gamestate *gamestate)
{
    transition_to_play(gamestate);
}

// any state --> reset
void transition_to_reset(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->timer = timer;
}

void reset_init(void *game_state)
{
}

void reset_update(void *game_state, float dt)
{
}

void reset_render(void *game_state)
{
}
