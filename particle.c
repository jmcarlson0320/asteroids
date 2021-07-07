#include "particle.h"
#include <tiny-fw.h>
#include <stdlib.h>
#include <math.h>

Emitter emitter_create(int n, int x, int y)
{
    Emitter e;
    e.particles = malloc(n * sizeof(Particle));
    e.num_particles = n;
    e.num_active_particles = 0;
    e.pos.e[X_COOR] = (float) x;
    e.pos.e[Y_COOR] = (float) y;
    e.max_lifetime = 1.0f;
    e.emit_flag = 0;

    emitter_reset_particles(&e);

    return e;
}

void emitter_init(Emitter *e, int n, int x, int y)
{
    if (!e->particles) {
        e->particles = malloc(n * sizeof(Particle));
    }
    e->num_particles = n;
    e->num_active_particles = 0;
    e->pos.e[X_COOR] = (float) x;
    e->pos.e[Y_COOR] = (float) y;
    e->max_lifetime = 1.0f;
    e->emit_flag = 0;

    emitter_reset_particles(e);
}

void emitter_destroy(Emitter *e)
{
    free(e->particles);
}

void emitter_reset_particles(Emitter *e)
{
    for (int i = 0; i < e->num_particles; i++) {
        e->particles[i].active = 0;
        e->particles[i].pos = e->pos;
        float speed = (float) (rand() % 50);
        float angle = (float) (rand() % 360) * M_PI / 180.0f;
        e->particles[i].vel = new_vec2(speed * cos(angle), speed * sin(angle));
        e->particles[i].color = 0xffffff;
        e->particles[i].lifetime = ((float) rand() / (float) RAND_MAX) * e->max_lifetime;
    }
    e->num_active_particles = 0;
}

void emitter_start_emitting(Emitter *e)
{
    e->emit_flag = 0;
}

void emitter_stop_emitting(Emitter *e)
{
    e->emit_flag = 1;
}

void emitter_update(Emitter *e, float dt)
{
    for (int i = 0; i < e->num_particles; i++) {
        e->particles[i].lifetime += dt;
        if (e->particles[i].lifetime > e->max_lifetime) {
            e->particles[i].active = 1;
            e->particles[i].pos = e->pos;
            float speed = (float) (rand() % 50);
            float angle = (float) (rand() % 360) * M_PI / 180.0f;
            e->particles[i].vel = new_vec2(speed * cos(angle), speed * sin(angle));
            e->particles[i].color = 0xffffff;
            e->particles[i].lifetime = ((float) rand() / (float) RAND_MAX) * e->max_lifetime;
        } else {
            vec2 delta_pos;
            vec2_mult(&delta_pos, &e->particles[i].vel, dt);
            vec2_add(&e->particles[i].pos, &e->particles[i].pos, &delta_pos);
            float col_amt = norm(e->max_lifetime - e->particles[i].lifetime, 0, e->max_lifetime);
            e->particles[i].color = interpolate_color(col_amt, 0x000000, 0xFFFFFF);
        }
    }
}

void emitter_render(Emitter *e)
{
    if (e->num_active_particles > 0) {
        for (int i = 0; i < e->num_particles; i++) {
            if (e->particles[i].active) {
                draw_point(e->particles[i].pos.e[X_COOR], e->particles[i].pos.e[Y_COOR], e->particles[i].color);
            }
        }
    }
}
