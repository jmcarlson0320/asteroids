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

int main(int argc, char *argv[])
{
    App app = app_create(WIDTH, HEIGHT);

    List *points = list_new();
    enum mode mode = DRAW;
    vec2 *selected_point = NULL;
    vec2 *nearest_point = NULL;
    vec2 *next_point = NULL;

    app_start(&app);
    while (app.running) {
        app_update(&app);

        if (app.keyboard.down[KEY_Q]) {
            app.running = 0;
        }

        if (app.keyboard.pressed[KEY_D]) {
            mode = DRAW;
        }

        if (app.keyboard.pressed[KEY_E]) {
            mode = EDIT;
        }

        if (mode == DRAW) {
            nearest_point = closest_point(points, app.mouse.x, app.mouse.y);
            if (nearest_point) {
                int next_point_pos = list_index(points, nearest_point);
                next_point_pos++;
                next_point_pos %= list_len(points);
                next_point = list_get(points, next_point_pos);
            }
            if (app.mouse.pressed[MOUSE_BUTTON_LEFT]) {
                vec2 *point = malloc(sizeof(vec2));
                *point = new_vec2(app.mouse.x, app.mouse.y);
                if (next_point) {
                    list_insert(points, list_index(points, next_point), point);
                } else {
                    list_append(points, point);
                }
            }
        }

        if (mode == EDIT) {
            nearest_point = closest_point(points, app.mouse.x, app.mouse.y);
            if (app.mouse.pressed[MOUSE_BUTTON_LEFT]) {
                selected_point = nearest_point;
            }

            if (app.mouse.released[MOUSE_BUTTON_LEFT]) {
                selected_point = NULL;
            }

            if (app.mouse.button[MOUSE_BUTTON_LEFT]) {
                selected_point->e[X_COOR] = app.mouse.x;
                selected_point->e[Y_COOR] = app.mouse.y;
            }

            if (app.mouse.pressed[MOUSE_BUTTON_RIGHT]) {
                list_remove(points, nearest_point);
                nearest_point = closest_point(points, app.mouse.x, app.mouse.y);
            }
        }

        clear_screen();
        draw_points(points);

        if (mode == EDIT && nearest_point) {
            draw_fill_rect(nearest_point->e[X_COOR] - 1, nearest_point->e[Y_COOR] - 1, 
                           nearest_point->e[X_COOR] + 1, nearest_point->e[Y_COOR] + 1, 0x00ff00);
        }

        if (mode == DRAW && nearest_point) {
            draw_fill_rect(nearest_point->e[X_COOR] - 1, nearest_point->e[Y_COOR] - 1, 
                           nearest_point->e[X_COOR] + 1, nearest_point->e[Y_COOR] + 1, 0xffffff);
        }

        if (mode == DRAW && next_point) {
            draw_fill_rect(next_point->e[X_COOR] - 1, next_point->e[Y_COOR] - 1, 
                           next_point->e[X_COOR] + 1, next_point->e[Y_COOR] + 1, 0xffffff);
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
