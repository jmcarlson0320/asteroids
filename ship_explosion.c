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
    e->lifetime = 0.6f;
    e->active_flag = ACTIVE;
    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        ship_explosion_piece *p = &e->pieces[i];
        p->pos = e->pos;
        p->vel = vec2_unit_vec((float) rand() / (float) RAND_MAX * 2.0f * M_PI);
        vec2_mult(&p->vel, &p->vel, rand_float(5.0f, 50.0f));
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
        draw_fill_circle(p->pos.e[X_COOR], p->pos.e[Y_COOR], 1, 0xffffff);
    }
}
