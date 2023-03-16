#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static char *ship_model_filename = "models/ship.model";
static vec2 *ship_model;
static int num_vertices_ship_model;

static vec2 flame_model[3] = {
    {{-0.8f,  0.0f}},
    {{-0.1f,  0.1f}},
    {{-0.1f, -0.1f}}
};

void load_ship_model()
{
    FILE *fp = fopen(ship_model_filename, "r");
    if (!fp) {
        printf("could not open ship model file %s\n", ship_model_filename);
        exit(1);
    }

    num_vertices_ship_model = 0;
    fscanf(fp, "%d\n", &num_vertices_ship_model);

    ship_model = malloc(sizeof(vec2) * num_vertices_ship_model);

    for (int i = 0; i < num_vertices_ship_model; i++)
        fscanf(fp, "%f %f\n", &ship_model[i].e[X_COOR], &ship_model[i].e[Y_COOR]);

    fclose(fp);
}

void free_ship_model()
{
    free(ship_model);
}

void ship_init(ship *s, float x, float y)
{
    load_ship_model();
    s->angle = 3.0f * M_PI / 2.0f;
    s->scale = 10;
    s->pos = new_vec2(x, y);
    s->vel = new_vec2(0, 0);
    s->drag = 0.99f;
    s->flame_timer = 0;
    s->flame_toggle = 1;
    s->ctl_rotate = ROTATE_STOP;
    s->ctl_thrust = 0;
}

static void turn_ship(ship *s)
{
    if (s->ctl_rotate == ROTATE_LEFT)
        s->angle -= 0.08;
    else if (s->ctl_rotate == ROTATE_RIGHT)
        s->angle += 0.08;
}

static void engage_thrust(ship *s, float dt)
{
    if (!s->ctl_thrust)
        return;

    vec2 dv = new_vec2(cos(s->angle), sin(s->angle));
    vec2_normalize(&dv, &dv);
    vec2_mult(&dv, &dv, 4);
    vec2_add(&s->vel, &s->vel, &dv);
    s->flame_timer += dt;
    if (s->flame_timer > 0.066f) {
        s->flame_timer = 0;
        s->flame_toggle = !s->flame_toggle;
    }
}

static void apply_drag(ship *s)
{
    vec2_mult(&s->vel, &s->vel, s->drag);
}

static void move_ship(ship *s, float dt)
{
    vec2 ds;
    vec2_mult(&ds, &s->vel, dt);
    vec2_add(&s->pos, &s->pos, &ds);
    s->pos = wrap_coor(s->pos, WIDTH, HEIGHT);
}

void ship_update(ship *s, float dt)
{
    turn_ship(s);
    engage_thrust(s, dt);
    apply_drag(s);
    move_ship(s, dt);
}

void draw_ship_wireframe(int x, int y)
{
    transform t = new_transform();
    transform_scale(&t, 10);
    transform_rotate(&t, - M_PI / 2);
    transform_translate(&t, x, y);
    draw_wireframe(ship_model, num_vertices_ship_model, &t);
}

void ship_render(ship *s)
{
    transform t = new_transform();
    transform_scale(&t, s->scale);
    transform_rotate(&t, s->angle);
    transform_translate(&t, s->pos.e[X_COOR], s->pos.e[Y_COOR]);
    draw_wireframe(ship_model, num_vertices_ship_model, &t);

    if (s->ctl_thrust && s->flame_toggle)
        draw_wireframe(flame_model, 3, &t);
}
