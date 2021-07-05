#include "defs.h"

#define PAUSE_LENGTH 3.0f

static float timer_sec = 0;
static float timer_display = 0;
static int display_flag = ACTIVE;

static void timer(asteroids *game)
{
    transition_to_play(game);
}

static void reset_update(asteroids *game, float dt)
{
    timer_sec += dt;
    timer_display += dt;
    if (timer_sec >= PAUSE_LENGTH) {
        timer(game);
    }

    if (timer_display >= 0.5f) {
        timer_display = 0.0f;
        display_flag = !display_flag;
    }
}

static void reset_render(asteroids *game)
{
    clear_screen();
    if (display_flag) {
        draw_text("GET READY!!!", WIDTH / 2 - (7 * 12 / 2), HEIGHT / 2.5, 0xffffff);
    }
}

void transition_to_reset(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.timer = timer;
    game->gamestate.update = reset_update;
    game->gamestate.render= reset_render;

    timer_sec = 0.0f;
    timer_display = 0.0f;
    display_flag = ACTIVE;
}
