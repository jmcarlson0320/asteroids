#include "defs.h"


static void control_rotation(ship *s, int inputs[])
{
    if (inputs[LEFT] && inputs[RIGHT]) {
        s->ctl_rotate = ROTATE_STOP;
    } else if (inputs[LEFT]) {
        s->ctl_rotate = ROTATE_LEFT;
    } else if (inputs[RIGHT]) {
        s->ctl_rotate = ROTATE_RIGHT;
    } else {
        s->ctl_rotate = ROTATE_STOP;
    }
}


static void control_thrust(ship *s, int inputs[])
{
    if (inputs[THRUST]) {
        s->ctl_thrust = 1;
    } else {
        s->ctl_thrust = 0;
    }
}


static void control_ship(ship *s, int inputs[])
{
    control_rotation(s, inputs);
    control_thrust(s, inputs);
}


static void test_update(asteroids *game, float dt)
{
    int *inputs = game->input;
    ship *s = &game->player;

    control_ship(s, inputs);
    ship_update(&game->player, dt);

    // fire bullets
    if (game->input[FIRE])
        fire_bullet(&game->bullet_list, game->player.pos, game->player.vel, game->player.angle);

    if (inputs[START_GAME])
        spawn_enemy(&game->enemy);

    target_player_position(game->player.pos);
    enemy_update(&game->enemy, dt);
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
