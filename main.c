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

void pre_process(int *image, vec3 *pre_process, int w, int h)
{
    for (int j = 0; j < h; j++) {
        vec3 row_total = new_vec3(0, 0, 0);
        for (int i = 0; i < w; i++) {
            int pixel = image[j * w + i];
            row_total.e[RED_COMP] += (pixel & 0xff0000) >> 16;
            row_total.e[GREEN_COMP] += (pixel & 0x00ff00) >> 8;
            row_total.e[BLUE_COMP] += (pixel & 0x0000ff);
            pre_process[j * w + i] = row_total;
            if (j >= 1) {
                vec3_add(&pre_process[j * w + i], &pre_process[j * w + i], &pre_process[(j - 1) * w + i]);
            }
        }
    }
}

vec3 lookup(vec3 *pre_process, int x, int y, int w, int h)
{
    if (x < 0)
        x = 0;
    else if (x >= w)
        x = w - 1;
    if (y < 0)
        y = 0;
    else if (y >= h)
        y = h - 1;

    return pre_process[y * w + x];
}

void blur_fast(int *image, int *result, int w, int h, int k, int c)
{
    vec3 *p = malloc(sizeof(vec3) * w * h);
    pre_process(image, p, w, h);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            vec3 p1 = lookup(p, i + k, j + k, w, h);
            vec3 p2 = lookup(p, i - k, j - k, w, h);
            vec3 p3 = lookup(p, i + k, j - k, w, h);
            vec3 p4 = lookup(p, i - k, j + k, w, h);

            vec3_add(&p1, &p1, &p2);
            vec3_sub(&p1, &p1, &p3);
            vec3_sub(&p1, &p1, &p4);

            float scale = (2 * k + 1) * (2 * k + 1) / c;
            vec3_div(&p1, &p1, scale);

            int r, g, b;
            r = p1.e[RED_COMP];
            g = p1.e[GREEN_COMP];
            b = p1.e[BLUE_COMP];

            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            result[j * w + i] = r << 16 | g << 8 | b;
        }
    }
    free(p);
}

int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT);

    asteroids *game = malloc(sizeof(asteroids));
    asteroids_init(game);

    int *pixels = malloc(sizeof(int) * WIDTH * HEIGHT);
    Bitmap blurred = bitmap_create(WIDTH, HEIGHT, pixels);

    int BLOOM = 1;
    int bloom_spread = 5;
    float bloom_intensity = 1;

    app_start(&app);
    while (app.running) {
        app_update(&app);

        get_user_input(game, &app);

        if (app.keyboard.pressed[KEY_UP]) {
            bloom_spread++;
        }
        if (app.keyboard.pressed[KEY_DOWN]) {
            bloom_spread--;
        }
        if (app.keyboard.pressed[KEY_LEFT]) {
            bloom_intensity--;
            if (bloom_intensity <= 0) {
                bloom_intensity = 1;
            }
        }
        if (app.keyboard.pressed[KEY_RIGHT]) {
            bloom_intensity++;
        }
        if (app.keyboard.pressed[KEY_B]) {
            BLOOM = !BLOOM;
        }

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
