#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static char *ship_model_filename = "models/ship.model";

static vec2 *ship_model;
static int num_points_ship_model;

static vec2 flame_model[3] = {
    {{-0.5f,  0.0f}},
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

    num_points_ship_model = 0;
    fscanf(fp, "%d\n", &num_points_ship_model);

    ship_model = malloc(sizeof(vec2) * num_points_ship_model);

    // for each point
    for (int i = 0; i < num_points_ship_model; i++) {
        //      read it into the models array
        fscanf(fp, "%f %f\n", &ship_model[i].e[X_COOR], &ship_model[i].e[Y_COOR]);
    }
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

void ship_update(ship *s, float dt)
{
    switch (s->ctl_rotate) {
        case ROTATE_LEFT:
            s->angle -= 0.06;
            break;
        case ROTATE_RIGHT:
            s->angle += 0.06;
            break;
        default:
            break;
    }

    if (s->ctl_thrust) {
        vec2 dv = new_vec2(cos(s->angle), sin(s->angle));
        vec2_normalize(&dv, &dv);
        vec2_mult(&dv, &dv, 3);
        vec2_add(&s->vel, &s->vel, &dv);
        s->flame_timer += dt;
        if (s->flame_timer > 0.066f) {
            s->flame_timer = 0;
            s->flame_toggle = !s->flame_toggle;
        }
    }

    vec2_mult(&s->vel, &s->vel, s->drag);

    vec2 ds;
    vec2_mult(&ds, &s->vel, dt);
    vec2_add(&s->pos, &s->pos, &ds);

    s->pos = wrap_coor(s->pos, WIDTH, HEIGHT);
}

void draw_ship_wireframe(int x, int y)
{
    transform t = new_transform();
    transform_scale(&t, 10);
    transform_rotate(&t, - M_PI / 2);
    transform_translate(&t, x, y);
    draw_wireframe(ship_model, num_points_ship_model, &t);
}

void ship_render(ship *s)
{
    // build transformation, needs to be in this order
    transform t = new_transform();
    transform_scale(&t, s->scale);
    transform_rotate(&t, s->angle);
    transform_translate(&t, s->pos.e[X_COOR], s->pos.e[Y_COOR]);
    draw_wireframe(ship_model, num_points_ship_model, &t);

    if (s->ctl_thrust && s->flame_toggle) {
        draw_wireframe(flame_model, 3, &t);
    }
}
