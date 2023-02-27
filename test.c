#include "defs.h"

static void test_update(asteroids *game, float dt)
{

    if (game->input[START_GAME]) {
        spawn_enemy(&game->enemy);
    }

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

    target_player_position(game->player.pos);
    enemy_update(&game->enemy, dt);
    ship_update(&game->player, dt);
    update_bullets(&game->bullet_list, dt);
}


static void test_render(asteroids *game)
{
    enemy_render(&game->enemy);
    ship_render(&game->player);

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet b = game->bullet_list.bullets[i];
        if (b.active_flag) {
            draw_fill_circle(b.pos.e[X_COOR], b.pos.e[Y_COOR], 1, 0xffffff);
        }
    }
}


void transition_to_test(asteroids *game)
{
    game->update = test_update;
    game->render = test_render;
    game->state = TEST;
    spawn_enemy(&game->enemy);
}
