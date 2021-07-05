#include "defs.h"

// initials entered event
static void initials_entered(gamestate *gamestate)
{
    transition_to_title(gamestate);
}

// any state --> gameover
void transition_to_gameover(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->initials_entered = initials_entered;
}

void gameover_init(void *game_state)
{
}

void gameover_update(void *game_state, float dt)
{
}

void gameover_render(void *game_state)
{
}
