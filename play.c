#include "defs.h"

static void destroyed(asteroids *game)
{
    transition_to_reset(game);
}

static void gameover(asteroids *game)
{
    transition_to_gameover(game);
}

static void cleared(asteroids *game)
{
    transition_to_reset(game);
}

static void play_update(asteroids *game, float dt)
{
}

static void play_render(asteroids *game)
{
    clear_screen();
    draw_text("play state", 0, 0, 0xffff00);
}

void transition_to_play(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.destroyed = destroyed;
    game->gamestate.gameover = gameover;
    game->gamestate.cleared = cleared;
    game->gamestate.update = play_update;
    game->gamestate.render = play_render;
}
