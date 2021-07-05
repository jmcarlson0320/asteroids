#include "defs.h"

// start game event
static void start(asteroids *game)
{
    transition_to_reset(game);
}

static void title_update(asteroids *game, float dt)
{
}

static void title_render(asteroids *game)
{
    clear_screen();
    draw_text("title state", 0, 0, 0x0000ff);
}

void transition_to_title(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.start = start;
    game->gamestate.update = title_update;
    game->gamestate.render = title_render;
}
