#include "defs.h"
#include <math.h>
#include <stdlib.h>

void ship_explosion_init(ship_explosion *e)
{
    e->active_flag = INACTIVE;
}

void ship_explosion_start(ship_explosion *e, int x, int y)
{
    e->pos = new_vec2(x, y);
    e->lifetime = 1.5f;
    e->active_flag = ACTIVE;
    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        ship_explosion_piece *p = &e->pieces[i];
        p->pos = e->pos;
        p->vel = vec2_unit_vec((float) rand() / (float) RAND_MAX * 2.0f * M_PI);
        vec2_mult(&p->vel, &p->vel, rand_float(5.0f, 20.0f));
        p->angle = rand_float(0, 6.28);
        p->angular_vel = rand_float(0, 3);
    }
}

void ship_explosion_update(ship_explosion *e, float dt)
{
    if (!e->active_flag) {
        return;
    }

    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        ship_explosion_piece *p = &e->pieces[i];
        vec2 ds;
        vec2_mult(&ds, &p->vel, dt);
        vec2_add(&p->pos, &p->pos, &ds);
        p->angle = p->angle + p->angular_vel * dt;
    }

    e->lifetime -= dt;
    if (e->lifetime <= 0) {
        e->active_flag = INACTIVE;
    }
}

void ship_explosion_render(ship_explosion *e)
{
    if (!e->active_flag) {
        return;
    }

    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        ship_explosion_piece *p = &e->pieces[i];
        vec2 p1 = vec2_unit_vec(p->angle);
        vec2 p2 = vec2_unit_vec(p->angle + M_PI);
        vec2_mult(&p1, &p1, 3);
        vec2_mult(&p2, &p2, 3);
        vec2_add(&p1, &p1, &p->pos);
        vec2_add(&p2, &p2, &p->pos);
        float x1 = p1.e[X_COOR];
        float x2 = p2.e[X_COOR];
        float y1 = p1.e[Y_COOR];
        float y2 = p2.e[Y_COOR];
        draw_line(x1, y1, x2, y2, 0xffffff);
    }
}
