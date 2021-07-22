#include "defs.h"

static void initials_entered(asteroids *game)
{
    transition_to_title(game);
}

static void gameover_update(asteroids *game, float dt)
{
}

static void gameover_render(asteroids *game)
{
    clear_screen();
    draw_text("gameover state", 0, 0, 0xff0000);
}

void transition_to_gameover(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.update = gameover_update;
    game->gamestate.render = gameover_render;
}

