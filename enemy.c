#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static char *enemy_model_filename = "models/enemy.model";

static vec2 *enemy_model;
static int num_points_enemy_model;

void load_enemy_model()
{
    FILE *fp = fopen(enemy_model_filename, "r");
    if (!fp) {
        printf("could not open enemy model file %s\n", enemy_model_filename);
        exit(1);
    }

    num_points_enemy_model = 0;
    fscanf(fp, "%d\n", &num_points_enemy_model);

    enemy_model = malloc(sizeof(vec2) * num_points_enemy_model);

    for (int i = 0; i < num_points_enemy_model; i++) {
        fscanf(fp, "%f %f\n", &enemy_model[i].e[X_COOR], &enemy_model[i].e[Y_COOR]);
    }
    fclose(fp);
}

void free_enemy_model()
{
    free(enemy_model);
}

void enemy_init(enemy *s, float x, float y)
{
    load_enemy_model();
    s->angle = 3.0f * M_PI / 2.0f;
    s->scale = 10;
    s->pos = new_vec2(x, y);

    s->vel = new_vec2(0, 0);
}

void enemy_update(enemy *s, float dt)
{
    vec2 ds;
    vec2_mult(&ds, &s->vel, dt);
    vec2_add(&s->pos, &s->pos, &ds);
}

void enemy_render(enemy *s)
{
    // build transformation, needs to be in this order
    transform t = new_transform();
    transform_scale(&t, s->scale);
    transform_rotate(&t, s->angle);
    transform_translate(&t, s->pos.e[X_COOR], s->pos.e[Y_COOR]);
    draw_wireframe(enemy_model, num_points_enemy_model, &t);
}
