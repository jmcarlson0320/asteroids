#include "defs.h"
#include <math.h>
#include <stdlib.h>

void explosion_init(explosion *e)
{
    e->active_flag = INACTIVE;
}

void explosion_start(explosion *e, int x, int y)
{
    e->pos = new_vec2(x, y);
    e->lifetime = 0.6f;
    e->active_flag = ACTIVE;
    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        explosion_piece *p = &e->pieces[i];
        p->pos = e->pos;
        p->vel = vec2_unit_vec(rand_float(0.0f, 6.28f));
        vec2_mult(&p->vel, &p->vel, rand_float(5.0f, 50.0f));
    }
}

void explosion_update(explosion *e, float dt)
{
    if (e->active_flag == INACTIVE) {
        return;
    }

    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        explosion_piece *p = &e->pieces[i];
        vec2 ds;
        vec2_mult(&ds, &p->vel, dt);
        vec2_add(&p->pos, &p->pos, &ds);
    }

    e->lifetime -= dt;
    if (e->lifetime <= 0) {
        e->active_flag = INACTIVE;
    }
}

void explosion_render(explosion *e)
{
    if (!e->active_flag) {
        return;
    }

    for (int i = 0; i < NUM_EXPLOSION_PARTICLES; i++) {
        explosion_piece *p = &e->pieces[i];
        draw_fill_circle(p->pos.e[X_COOR], p->pos.e[Y_COOR], 1, 0xffffff);
    }
}
