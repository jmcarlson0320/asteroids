#include "defs.h"

// destroyed event
static void destroyed(gamestate *gamestate)
{
    transition_to_reset(gamestate);
}

// gameover event
static void gameover(gamestate *gamestate)
{
    transition_to_gameover(gamestate);
}

// level_cleared event
static void cleared(gamestate *gamestate)
{
    transition_to_reset(gamestate);
}

// any state --> play
void transition_to_play(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->destroyed = destroyed;
    gamestate->gameover = gameover;
    gamestate->cleared = cleared;
}

void play_init(void *game_state)
{
}

void play_update(void *game_state, float dt)
{
}

void play_render(void *game_state)
{
}
