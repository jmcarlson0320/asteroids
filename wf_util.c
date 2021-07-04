#include <tiny-fw.h>
#include "list.h"
#include <stdlib.h>
#include <float.h>
#include <stdio.h>

#define SCALE_FACTOR 4
#define WIDTH (1920 / SCALE_FACTOR)
#define HEIGHT (1080 / SCALE_FACTOR)

enum mode {
    DRAW,
    EDIT
};

void usage()
{
    printf("usage: wf <output filename>\n");
}

void clear_screen()
{
    draw_fill_rect(0, 0, WIDTH - 1, HEIGHT - 1, 0x000000);
}

void draw_cursor(int x, int y)
{
    draw_line(x - 2, y, x + 2, y, 0xffffff);
    draw_line(x, y - 2, x, y + 2, 0xffffff);
    draw_point(x, y, 0xff0000);
}

void draw_points(List *points)
{
    List_Iterator it = list_iterator(points);
    vec2 *prev = NULL;
    vec2 *next = NULL;
    while (list_has_next(&it)) {
        next = list_next(&it);
        if (prev) {
            draw_line(prev->e[X_COOR], prev->e[Y_COOR], next->e[X_COOR], next->e[Y_COOR], 0xffffff);
        }
        draw_point(next->e[X_COOR], next->e[Y_COOR], 0xffffff);
        prev = next;
    }

    // close the shape
    vec2 *first = list_get(points, 0);
    vec2 *last = list_get(points, list_len(points) - 1);
    if (first && last) {
        draw_line(first->e[X_COOR], first->e[Y_COOR], last->e[X_COOR], last->e[Y_COOR], 0xffffff);
    }
}

float dist(vec2 *u, vec2 *v)
{
    vec2 dist_vec;
    vec2_sub(&dist_vec, u, v);
    return vec2_len(&dist_vec);
}

float min(float a, float b)
{
    return a <= b ? a : b;
}

float max(float a, float b)
{
    return a >= b ? a : b;
}

vec2 average_points(List *points)
{
    vec2 sum = new_vec2(0, 0);
    List_Iterator it = list_iterator(points);
    while(list_has_next(&it)) {
        vec2 *cur = list_next(&it);
        vec2_add(&sum, &sum, cur);
    }

    vec2 avg;
    vec2_div(&avg, &sum, list_len(points));

    return avg;
}

vec2 *closest_point(List *points, int x, int y)
{
    vec2 test_point = new_vec2(x, y);
    vec2 *closest = NULL;
    float closest_dist = FLT_MAX;

    List_Iterator it = list_iterator(points);
    vec2 *next;
    while((next = list_next(&it))) {
        float next_dist = dist(next, &test_point);
        if (next_dist < closest_dist) {
            closest = next;
            closest_dist = next_dist;
        }
    }
    return closest;
}

vec2 *furthest_point(List *points, int x, int y)
{
    vec2 test_point = new_vec2(x, y);
    vec2 *furthest = NULL;
    float furthest_dist = 0;

    List_Iterator it = list_iterator(points);
    vec2 *next;
    while((next = list_next(&it))) {
        float next_dist = dist(next, &test_point);
        if (next_dist > furthest_dist) {
            furthest = next;
            furthest_dist = next_dist;
        }
    }
    return furthest;
}

void center_points(List *points)
{
    // fing avg
    vec2 avg = average_points(points);

    // subtract average from every point
    List_Iterator it = list_iterator(points);
    while(list_has_next(&it)) {
        vec2 *cur = list_next(&it);
        vec2_sub(cur, cur, &avg);
    }
}

void normalize_points(List *points)
{
    // find avg
    vec2 avg = average_points(points);

    // find point furthest from center
    vec2 *furthest = furthest_point(points, avg.e[X_COOR], avg.e[Y_COOR]);

    // get distance from furthest to center
    vec2 diff;
    vec2_sub(&diff, furthest, &avg);
    float dist = vec2_len(&diff);

    // get 1/length
    float scale_factor = 1.0f / dist;

    // scale all points by 1/length
    List_Iterator it = list_iterator(points);
    while(list_has_next(&it)) {
        vec2 *cur = list_next(&it);
        vec2_mult(cur, cur, scale_factor);
    }
}

typedef struct {
    List *points;
    int num_points;
} model;

void write_to_file(model *m, char *filename)
{
    FILE *fp = fopen(filename, "w");
    fprintf(fp, "%d\n", m->num_points);
    for (int i = 0; i < m->num_points; i++) {
        vec2 *p = list_get(m->points, i);
        fprintf(fp, "%f %f\n", p->e[X_COOR], p->e[Y_COOR]);
    }
    fclose(fp);
}

model read_from_file(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("could not open file %s\n", filename);
        exit(1);
    }
    model m;
    m.points = list_new();
    m.num_points = 0;
    fscanf(fp, "%d\n", &m.num_points);
    for (int i = 0; i < m.num_points; i++) {
        vec2 *point = malloc(sizeof(vec2));
        fscanf(fp, "%f %f\n", &point->e[X_COOR], &point->e[Y_COOR]);
        list_append(m.points, point);
    }
    fclose(fp);
    return m;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage();
        return -1;
    }

    char *output_filename = argv[1];
    List *points = list_new();
    enum mode mode = DRAW;
    vec2 *selected_point = NULL;
    vec2 *nearest_point = NULL;

    App app = app_create(WIDTH, HEIGHT);
    app_start(&app);
    while (app.running) {
        app_update(&app);

        if (app.keyboard.down[KEY_Q]) {
            app.running = 0;
            center_points(points);
            normalize_points(points);
            model m;
            m.points = points;
            m.num_points = list_len(points);
            write_to_file(&m, output_filename);
        }

        if (app.keyboard.pressed[KEY_D]) {
            mode = DRAW;
        }

        if (app.keyboard.pressed[KEY_E]) {
            mode = EDIT;
        }

        nearest_point = closest_point(points, app.mouse.x, app.mouse.y);

        if (mode == DRAW) {
            if (app.mouse.pressed[MOUSE_BUTTON_LEFT]) {
                vec2 *point = malloc(sizeof(vec2));
                *point = new_vec2(app.mouse.x, app.mouse.y);
                int index = list_index(points, nearest_point);
                if (index != -1) {
                    // there are points in the list, insert at index
                    list_insert(points, index, point);
                } else {
                    // list is empty, just append new point
                    list_append(points, point);
                }
            }
        }

        if (mode == EDIT) {
            if (app.mouse.pressed[MOUSE_BUTTON_LEFT]) {
                selected_point = nearest_point;
            }

            if (app.mouse.released[MOUSE_BUTTON_LEFT]) {
                selected_point = NULL;
            }

            if (app.mouse.button[MOUSE_BUTTON_LEFT] && selected_point) {
                selected_point->e[X_COOR] = app.mouse.x;
                selected_point->e[Y_COOR] = app.mouse.y;
            }

            if (app.mouse.pressed[MOUSE_BUTTON_RIGHT]) {
                if (nearest_point) {
                    list_remove(points, nearest_point);
                    nearest_point = closest_point(points, app.mouse.x, app.mouse.y);
                }
            }
        }

        clear_screen();
        draw_points(points);

        // highlight the closest point
        if (nearest_point) {
            if (mode == EDIT) {
                draw_fill_rect(nearest_point->e[X_COOR] - 1, nearest_point->e[Y_COOR] - 1, 
                               nearest_point->e[X_COOR] + 1, nearest_point->e[Y_COOR] + 1, 0x00ff00);
            }
            if (mode == DRAW) {
                draw_fill_rect(nearest_point->e[X_COOR] - 1, nearest_point->e[Y_COOR] - 1, 
                               nearest_point->e[X_COOR] + 1, nearest_point->e[Y_COOR] + 1, 0xffffff);
            }
        }

        draw_cursor(app.mouse.x, app.mouse.y);

        if (mode == EDIT) {
            draw_text("EDIT", 0, 0, 0x00ff00);
        } else if (mode == DRAW) {
            draw_text("DRAW", 0, 0, 0xffffff);
        }

        app_draw_graphics(&app);
    }

    app_quit(&app);
}
