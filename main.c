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

    asteroids *game = malloc(sizeof(asteroids));
    asteroids_init(game);

    app_start(&app);
    while (app.running) {
        app_update(&app);

        get_user_input(game, &app);

        asteroids_update(game, app.time.dt_sec);
        asteroids_render(game);

        app_draw_graphics(&app);
    }
    app_quit(&app);

    return 0;
}
