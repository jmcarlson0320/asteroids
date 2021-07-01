#include "defs.h"
#include <math.h>

void ship_init(ship *s, vec2 *ship_model, vec2 *flame_model, float x, float y)
{
    s->model = ship_model;
    s->flame = flame_model;
    s->angle = 3.0f * M_PI / 2.0f;
    s->scale = 15;
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
            s->angle -= 0.05;
            break;
        case ROTATE_RIGHT:
            s->angle += 0.05;
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

void ship_render(ship *s)
{
    // build transformation, needs to be in this order
    transform t = new_transform();
    transform_scale(&t, s->scale);
    transform_rotate(&t, s->angle);
    transform_translate(&t, s->pos.e[X_COOR], s->pos.e[Y_COOR]);
    draw_wireframe(s->model, 3, &t);

    if (s->ctl_thrust && s->flame_toggle) {
        draw_wireframe(s->flame, 3, &t);
    }
}
