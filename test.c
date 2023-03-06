#include "defs.h"
#include <math.h>
#include <stdio.h>


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
    enemy *e = &game->enemy;

    control_ship(s, inputs);
    ship_update(s, dt);

    if (inputs[FIRE])
        fire_bullet(&game->bullet_list, s->pos, s->vel, s->angle);

    if (inputs[START_GAME])
        spawn_enemy(e);

    // enemy takes a shot
    e->shot_timer += dt;
    if (e->shot_timer >= 1) {
        vec2 shot_direction;
        vec2_sub(&shot_direction, &game->player.pos, &e->pos);
        float shot_angle = atan2(shot_direction.e[Y_COOR], shot_direction.e[X_COOR]);
        fire_bullet(&game->enemy_bullets, e->pos, new_vec2(0, 0), shot_angle);
        e->shot_timer = 0;
    }

    enemy_update(&game->enemy, dt);
    update_bullets(&game->bullet_list, dt);
    update_bullets(&game->enemy_bullets, dt);
}


static void test_render(asteroids *game)
{
    enemy_render(&game->enemy);
    ship_render(&game->player);

    render_bullets(&game->bullet_list);
    render_bullets(&game->enemy_bullets);
}


void transition_to_test(asteroids *game)
{
    game->update = test_update;
    game->render = test_render;
    game->state = TEST;
    spawn_enemy(&game->enemy);
}
