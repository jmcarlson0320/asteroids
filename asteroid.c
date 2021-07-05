#include "defs.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

char *filenames[3] = {
    "models/asteroid_01.model",
    "models/asteroid_02.model",
    "models/asteroid_03.model"
};

int asteroid_scales[NUM_TYPES] = {
    5,
    10,
    20
};

vec2 asteroid_models[NUM_MODELS][11];

void load_models()
{
    for (int i = 0; i < NUM_MODELS; i++) {
        // open the file
        FILE *fp = fopen(filenames[i], "r");
        if (!fp) {
            printf("could not open asteroid file %s\n", filenames[i]);
            exit(1);
        }

        int num_points = 0;
        fscanf(fp, "%d\n", &num_points);
        if (num_points != NUM_POINTS_ASTEROID) {
            printf("invalid model file %s\n", filenames[i]);
            exit(1);
        }

        // for each point
        for (int j = 0; j < 11; j++) {
        //      read it into the models array
            fscanf(fp, "%f %f\n", &asteroid_models[i][j].e[X_COOR], &asteroid_models[i][j].e[Y_COOR]);
        }
        fclose(fp);
    }
}

void asteroid_init(asteroid *a, enum asteroid_type type)
{
    a->vel = new_vec2((float) rand() / (float) RAND_MAX * 100.0f - 50.0f, (float) rand() / (float) RAND_MAX * 100.0f - 50.0f);
    a->angle = (float) rand() / (float) RAND_MAX * 2.0f * M_PI;
    a->ang_vel = (float) rand() / (float) RAND_MAX * 0.02f - 0.01f;
    a->pos = new_vec2((float) rand() / (float) RAND_MAX * WIDTH, (float) rand() / (float) RAND_MAX * HEIGHT);
    a->type = type;
    a->model = rand() % NUM_MODELS;
}

void asteroid_update(asteroid *a, float dt)
{
    a->angle += a->ang_vel;

    vec2 ds;
    vec2_mult(&ds, &a->vel, dt);
    vec2_add(&a->pos, &a->pos, &ds);

    a->pos = wrap_coor(a->pos, WIDTH, HEIGHT);
}

void asteroid_render(asteroid *a)
{
    // build transformation, needs to be in this order
    transform t = new_transform();
    transform_scale(&t, asteroid_scales[a->type]);
    transform_rotate(&t, a->angle);
    transform_translate(&t, a->pos.e[X_COOR], a->pos.e[Y_COOR]);
    draw_wireframe(asteroid_models[a->model], NUM_POINTS_ASTEROID, &t);
}
