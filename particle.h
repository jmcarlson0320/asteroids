#ifndef PARTICLE_H
#define PARTICLE_H

#include <tiny-fw.h>

typedef struct particle {
    vec2 pos;
    vec2 vel;
    int color;
    float lifetime;
    int active;
} Particle;

typedef struct emitter {
    Particle *particles;
    int num_particles;
    vec2 pos;
    float max_lifetime;
} Emitter;

Emitter emitter_create(int n, int x, int y);
void emitter_destroy(Emitter *e);
void emitter_reset_particles(Emitter *e);
void emitter_update(Emitter *e, float dt);
void emitter_render(Emitter *e);

#endif // PARTICLE_H
