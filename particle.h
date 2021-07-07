#ifndef PARTICLE_H
#define PARTICLE_H

#include <tiny-fw.h>

typedef struct particle {
    vec2 pos;
    vec2 vel;
    int color;
    float lifetime;
    int active;
    int starting;
} Particle;

typedef struct emitter {
    Particle *particles;
    int num_particles;
    int num_active_particles;
    vec2 pos;
    float max_lifetime;
    int emit_flag;
} Emitter;

Emitter emitter_create(int n, int x, int y);
void emitter_init(Emitter *e, int n, int x, int y);
void emitter_destroy(Emitter *e);
void emitter_reset_particles(Emitter *e);
void emitter_start_emitting(Emitter *e);
void emitter_stop_emitting(Emitter *e);
void emitter_update(Emitter *e, float dt);
void emitter_render(Emitter *e);

#endif // PARTICLE_H
