#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static char *enemy_model_filename = "models/enemy.model";

static vec2 *enemy_model;
static int num_points_enemy_model;
static vec2 last_known_player_position;

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

void enemy_init(enemy *e)
{
    load_enemy_model();
    e->angle = 3.0f * M_PI / 2.0f;
    e->scale = 10;
    e->pos = new_vec2(0, 0);
    e->vel = new_vec2(0, 0);
}

void enemy_update(enemy *e, float dt)
{
    if (!e->active) {
        return;
    }

    vec2 ds;
    vec2_mult(&ds, &e->vel, dt);
    vec2_add(&e->pos, &e->pos, &ds);

    e->shot_timer += dt;
    if (e->shot_timer >= 1) {
        // how should we take a shot??
        // track enemy bullets separate from player bullets
        // enemy struct includes enemy bullets array?
        e->shot_timer = 0;
    }

    if (e->pos.e[X_COOR] >= WIDTH) {
        e->active = 0;
    }
}

void enemy_render(enemy *e)
{
    transform t = new_transform();
    transform_scale(&t, e->scale);
    transform_rotate(&t, e->angle);
    transform_translate(&t, e->pos.e[X_COOR], e->pos.e[Y_COOR]);
    draw_wireframe(enemy_model, num_points_enemy_model, &t);
}

void spawn_enemy(enemy *e)
{
    e->pos = new_vec2(0, 100);
    e->vel = new_vec2(ENEMY_SPEED, 0);
    e->shot_timer = 0;
    e->active = ACTIVE;
}

void record_player_position(float x, float y)
{
    last_known_player_position.e[X_COOR] = x;
    last_known_player_position.e[Y_COOR] = y;
}
