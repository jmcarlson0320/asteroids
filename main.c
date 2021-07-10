#include <tiny-fw.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "list.h"
#include "particle.h"
#include "defs.h"

int convolve(int *image, int w, int h, int k, int x, int y)
{
    int r, g, b;
    for (int j = -k; j <= k; j++) {
        for (int i = -k; i <= k; i++) {
            int x_ = x + i;
            int y_ = y + j;
            if (x_ < 0 || w <= x_ || y_ < 0 || h <= y_) break;
            int pixel = image[y_ * w + x_];
            r += (pixel & 0xff0000) >> 16;
            g += (pixel & 0x00ff00) >> 8;
            b += (pixel & 0x0000ff);
        }
    }
    int k_2 = (2 * k + 1) * (2 * k + 1);
    r /= k_2;
    g /= k_2;
    b /= k_2;
    if (r > 0xff) r = 0xff;
    if (g > 0xff) g = 0xff;
    if (b > 0xff) b = 0xff;
    return r << 16 | g << 8 | b;
}

void blur(int *image, int *result, int w, int h, int k)
{
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            result[j * w + i] = convolve(image, w, h, k, i, j);
        }
    }
}

int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT);

    asteroids *game = malloc(sizeof(asteroids));
    asteroids_init(game);

    int *pixels = malloc(sizeof(int) * WIDTH * HEIGHT);
    Bitmap blurred = bitmap_create(WIDTH, HEIGHT, pixels);

    app_start(&app);
    while (app.running) {
        app_update(&app);

        get_user_input(game, &app);

        asteroids_update(game, app.time.dt_sec);
        clear_screen();
        asteroids_render(game);

        
        blur(app.graphics.pixels_rgb, pixels, WIDTH, HEIGHT, 3);
        draw_bitmap(&blurred, 0, 0, WIDTH - 1, HEIGHT - 1, 0, 0);

        asteroids_render(game);

        app_draw_graphics(&app);
    }
    app_quit(&app);

    asteroids_shutdown(game);
    free(game);

    return 0;
}
