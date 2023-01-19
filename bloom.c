#include <stdlib.h>
#include "tiny-fw.h"
#include "defs.h"
#include "bloom.h"

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

            int r = p1.e[RED_COMP];
            int g = p1.e[GREEN_COMP];
            int b = p1.e[BLUE_COMP];

            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            result[j * w + i] = r << 16 | g << 8 | b;
        }
    }
    free(p);
}

void bloom_debug_controls(int pressed[], int *BLOOM, int *spread, float *intensity)
{
    if (pressed[KEY_UP]) {
        (*spread)++;
    }

    if (pressed[KEY_DOWN]) {
        (*spread)--;
    }

    if (pressed[KEY_LEFT]) {
        (*intensity)--;
        if ((*intensity) <= 0) {
            (*intensity) = 1;
        }
    }

    if (pressed[KEY_RIGHT]) {
        (*intensity)++;
    }

    if (pressed[KEY_B]) {
        (*BLOOM) = !(*BLOOM);
    }
}

