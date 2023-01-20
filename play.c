#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

static void check_collisions(asteroids *game)
{
    // we shouldn't remove items from a list while iterating
    // track the asteroids to remove in this list, then remove them all at once
    List *to_remove = list_new();

    List_Iterator it = list_iterator(game->active_asteroids);
    while (list_has_next(&it)) {
        asteroid *a = list_next(&it);

        // bounding circles for ship and asteroid
        float asteroid_radius = asteroid_scales[a->type] * 0.9;
        vec2 *asteroid_origin = &a->pos;
        float ship_radius = game->player.scale * 0.7;
        vec2 *ship_origin = &game->player.pos;

        // check asteroid-ship collision
        if (circle_overlap(asteroid_origin, asteroid_radius, ship_origin, ship_radius)) {
            game->lives--;
            transition_to_reset(game);
            return;
        }

        // for each bullet
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullet *b = &game->bullet_list.bullets[i];
            if (b->active_flag) {
                vec2 *point = &b->pos;

                // check asteroid bullet collision
                if (point_in_circle(point, asteroid_origin, asteroid_radius)) {
                    // reset the bullet
                    b->active_flag = INACTIVE;
                    b->timer = 0;
                    game->bullet_list.num_bullets--;

                    list_append(to_remove, a);

                    // split the asteroid into smaller pieces
                    float x = a->pos.e[X_COOR];
                    float y = a->pos.e[Y_COOR];
                    if (a->type > SMALL) {
                        spawn_asteroid(game, x, y, a->type - 1);
                        spawn_asteroid(game, x, y, a->type - 1);
                    }

                    // start an explosion animation
                    explosion *e = find_inactive_explosion(game->explosion_list, MAX_EXPLOSIONS);
                    if (e) {
                        explosion_start(e, x, y);
                    }

                    game->score += asteroid_scores[a->type];
                }
            }
        }
    }

    // now reset all the asteroids in to_remove
    it = list_iterator(to_remove);
    while (list_has_next(&it)) {
        asteroid *a = list_next(&it);
        list_remove(game->active_asteroids, a);
        list_append(game->inactive_asteroids, a);
    }
    list_delete(to_remove);
}

static void play_update(asteroids *game, float dt)
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

    ship_update(&game->player, dt);
    update_asteroid_list(game->active_asteroids, dt);
    update_bullets(&game->bullet_list, dt);
    update_explosions(game->explosion_list, dt);

    check_collisions(game);
}

static void play_render(asteroids *game)
{
    ship_render(&game->player);

    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_render(a);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet b = game->bullet_list.bullets[i];
        if (b.active_flag) {
            draw_fill_circle(b.pos.e[X_COOR], b.pos.e[Y_COOR], 1, 0xffffff);
        }
    }

    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosion *e = &game->explosion_list[i];
        if (e->active_flag) {
            explosion_render(e);
        }
    }

    char score_string[MAX_SCORE_STRING_LENGTH];
    snprintf(score_string, MAX_SCORE_STRING_LENGTH, "%d", game->score);
    draw_text(score_string, 8, 5, 0xffffff);

    int x_offset = 8;
    int y_offset = 30;
    int spacing = 15;
    for (int i = 0; i < game->lives; i++) {
        draw_ship_wireframe(x_offset + i * spacing, y_offset);
    }
}

void transition_to_play(asteroids *game)
{
    game->update = play_update;
    game->render = play_render;

    game->player.vel = new_vec2(0, 0);
}
