#include "defs.h"

#define PAUSE_LENGTH 3.0f

static float timer_sec = 0;
static float timer_display = 0;
static int display_flag = ACTIVE;

static void start_init()
{
    timer_sec = 0.0f;
    timer_display = 0.0f;
    display_flag = ACTIVE;
}

static void start_update(asteroids *game, float dt)
{

    timer_sec += dt;
    if (timer_sec >= PAUSE_LENGTH)
        transition_to_play(game);

    timer_display += dt;
    if (timer_display >= 0.5f) {
        timer_display = 0.0f;
        display_flag = !display_flag;
    }
}

static void start_render(asteroids *game)
{
    if (display_flag) {
        draw_text("GET READY!!!", WIDTH / 2 - (7 * 12 / 2), HEIGHT / 2.5, 0xffffff);
    }
}

void transition_to_start(asteroids *game)
{
    start_init();
    game->update = start_update;
    game->render = start_render;
    game->state = START;
}
