#include "defs.h"

static void destroyed(gamestate *gamestate)
{
    transition_to_reset(gamestate);
}

static void gameover(gamestate *gamestate)
{
    transition_to_gameover(gamestate);
}

static void cleared(gamestate *gamestate)
{
    transition_to_reset(gamestate);
}

static void play_update(asteroids *game, float dt)
{
}

static void play_render(asteroids *game)
{
    clear_screen();
    draw_text("play state", 0, 0, 0xffff00);
}

void transition_to_play(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->destroyed = destroyed;
    gamestate->gameover = gameover;
    gamestate->cleared = cleared;
    gamestate->update = play_update;
    gamestate->render = play_render;
}
