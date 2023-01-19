#include <tiny-fw.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "list.h"
#include "particle.h"
#include "bloom.h"
#include "defs.h"

int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT, SCALE_FACTOR);

    asteroids *game = malloc(sizeof(asteroids));
    asteroids_init(game);

    int *pixels = malloc(sizeof(int) * WIDTH * HEIGHT);
    Bitmap blurred = bitmap_create(WIDTH, HEIGHT, pixels);

    int BLOOM = 1;
    int bloom_spread = 5;
    float bloom_intensity = 2;

    app_start(&app);
    while (app.running) {
        app_update(&app);

        get_user_input(game, &app);

        bloom_debug_controls(app.keyboard.pressed, &BLOOM, &bloom_spread, &bloom_intensity);
        asteroids_update(game, app.time.dt_sec);

        clear_screen();
        asteroids_render(game);
        if (BLOOM) {
            blur_fast(app.graphics.pixels_rgb, pixels, WIDTH, HEIGHT, bloom_spread, bloom_intensity);
            draw_bitmap(&blurred, 0, 0, WIDTH - 1, HEIGHT - 1, 0, 0);
            asteroids_render(game);
        }

        app_draw_graphics(&app);
    }
    app_quit(&app);

    asteroids_shutdown(game);
    free(game);

    return 0;
}
