#include "defs.h"

static void initials_entered(gamestate *gamestate)
{
    transition_to_title(gamestate);
}

static void gameover_update(asteroids *game, float dt)
{
}

static void gameover_render(asteroids *game)
{
    clear_screen();
    draw_text("gameover state", 0, 0, 0xff0000);
}

void transition_to_gameover(gamestate *gamestate)
{
    default_state(gamestate);
    gamestate->initials_entered = initials_entered;
    gamestate->update = gameover_update;
    gamestate->render = gameover_render;
}

