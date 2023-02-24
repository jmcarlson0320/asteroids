#include <stdio.h>
#include <stdlib.h>
#include <tiny-fw.h>
#include "defs.h"

App app;
asteroids *game;
int app_is_running = 0;

void init()
{
    app = app_create(WIDTH, HEIGHT, SCALE_FACTOR);
    app_start(&app);
    app_is_running = 1;

    game = malloc(sizeof(asteroids));
    asteroids_init(game);
}

void update()
{
    app_update(&app);
    if (app.keyboard.pressed[KEY_MAP[QUIT]]) {
        app_is_running = 0;
    }

    get_user_input(game, &app);
    asteroids_update(game, app.time.dt_sec);
}

void render()
{
    clear_screen();
    asteroids_render(game);
    app_draw_graphics(&app);
}

void cleanup()
{
    asteroids_shutdown(game);
    free(game);
    app_quit(&app);
}

int main()
{
    init();
    while (app_is_running) {
        update();
        render();
    }
    cleanup();

    return 0;
}
