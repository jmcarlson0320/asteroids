#include "defs.h"

static void test_update(asteroids *game, float dt)
{
    ship *s = &game->player;
    // ship rotation
    if (game->input[LEFT] && game->input[RIGHT]) {
        s->ctl_rotate = ROTATE_STOP;
    } else if (game->input[LEFT]) {
        s->ctl_rotate = ROTATE_LEFT;
    } else if (game->input[RIGHT]) {
        s->ctl_rotate = ROTATE_RIGHT;
    } else {
        s->ctl_rotate = ROTATE_STOP;
    }

    // ship thrust
    if (game->input[THRUST]) {
        s->ctl_thrust = 1;
    } else {
        s->ctl_thrust = 0;
    }

    // fire bullets
    if (game->input[FIRE] && game->bullet_list.num_bullets < MAX_BULLETS) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullet *b = &game->bullet_list.bullets[i];
            if (!b->active_flag) {
                b->active_flag = ACTIVE;
                b->timer = 0;
                b->pos = s->pos;
                b->vel = vec2_unit_vec(s->angle);
                vec2_mult(&b->vel, &b->vel, BULLET_SPEED);
                vec2_add(&b->vel, &b->vel, &s->vel);
                game->bullet_list.num_bullets++;
                break;
            }
        }
    }
    enemy_update(&game->enemy, dt);
    ship_update(&game->player, dt);
}


static void test_render(asteroids *game)
{
    enemy_render(&game->enemy);
    ship_render(&game->player);
}


void transition_to_test(asteroids *game)
{
    game->update = test_update;
    game->render = test_render;
    game->state = TEST;

    enemy_init(&game->enemy, 0, HEIGHT / 2);
}
