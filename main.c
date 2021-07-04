#include <tiny-fw.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list.h"
#include "particle.h"
#include "defs.h"

int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT);
    asteroids game;
    asteroids_init(&game);
    app_start(&app);
    while (app.running) {
        app_update(&app);
        get_user_input(&game, &app);

        // state transition
        game.event = check_for_event(&game);
        if (game.event != NO_EVENT) {
            game.state = next_state(game.state, game.event);
            game.event = NO_EVENT;
        }

        // the function pointers point to the update/render functions of the current gamestate
        (*game.update)(&game, app.time.dt_sec);
        (*game.render)(&game);

        app_draw_graphics(&app);
    }
    app_quit(&app);

    return 0;
}
