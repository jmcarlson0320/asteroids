#include "defs.h"
#include "bloom.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int COLORS[NUM_COLORS] = {
    [RED] = 0xff0000,
    [ORANGE] = 0xff7f00,
    [YELLOW] = 0xffff00,
    [GREEN] = 0x00ff00,
    [BLUE] = 0x0000ff,
    [INDIGO] = 0x4b0082,
    [VIOLET] = 0x7f00ff
};

const int KEY_MAP[NUM_INPUTS] = {
    [FIRE] = KEY_M,
    [LEFT] = KEY_Z,
    [RIGHT] = KEY_C,
    [THRUST] = KEY_N,
    [START_GAME] = KEY_RETURN,
    [TELEPORT] = KEY_SPACE,
    [QUIT] = KEY_Q,
    [NEXT_LETTER] = KEY_C,
    [PREV_LETTER] = KEY_Z
};

char *GAME_STATE_LABELS[NUM_GAMESTATES]  = {
    [TITLE] = "title",
    [START] = "start",
    [PLAY] = "play",
    [RESET] = "reset",
    [GAMEOVER] = "gameover",
    [HIGHSCORE] = "highscore"
};

void asteroids_init(asteroids *game)
{
    for (int i = 0; i < NUM_INPUTS; i++) {
        game->input[i] = 0;
    }

    srand(time(NULL));

    game->timer = 0.0f;
    game->cur_color = RED;

    ship_init(&game->player, WIDTH / 2, HEIGHT / 2);

    load_models();
    game->active_asteroids = list_new();
    game->inactive_asteroids = list_new();
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroid *a = malloc(sizeof(asteroid));
        asteroid_init(a, LARGE);
        list_append(game->inactive_asteroids, a);
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet *b = &game->bullet_list.bullets[i];
        b->pos = new_vec2(0, 0);
        b->vel = new_vec2(0, 0);
        b->active_flag = INACTIVE;
        b->timer = 0;
    }

    game->bullet_list.num_bullets = 0;
    game->score = 0;
    game->lives = 3;
    game->enemy_timer = 0;
    game->stage = 0;

    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosion *e = &game->explosion_list[i];
        explosion_init(e);
    }

    transition_to_title(game);
}

void get_user_input(asteroids *game, App *app)
{
    // key stays on while held, uses app->keyboard.down[]
    // ship controls use this style of input
    for (int i = 0; i < NUM_INPUTS; i++) {
        game->input[i] = app->keyboard.down[KEY_MAP[i]];
    }

    // key triggers once per press, uses app->keyboard.pressed[]
    // fire weapon uses this style of input
    game->input[FIRE] = app->keyboard.pressed[KEY_MAP[FIRE]];
    game->input[PREV_LETTER] = app->keyboard.pressed[KEY_MAP[PREV_LETTER]];
    game->input[NEXT_LETTER] = app->keyboard.pressed[KEY_MAP[NEXT_LETTER]];

    if (game->input[QUIT] == ACTIVE) {
        app->running = 0;
    }

#ifdef DEBUG
    debug_controls(game, app);
#endif // DEBUG
}

void asteroids_update(asteroids *game, float dt)
{
    game->update(game, dt);
}

void asteroids_render(asteroids *game)
{
    game->render(game);
    draw_text(GAME_STATE_LABELS[game->state], WIDTH / 2 - 20, HEIGHT - 10, 0xffffff);
}

void asteroids_shutdown(asteroids *game)
{
    List_Iterator it = list_iterator(game->active_asteroids);
    while (list_has_next(&it)) {
        free(list_next(&it));
    }

    list_delete(game->active_asteroids);
    it = list_iterator(game->inactive_asteroids);
    while (list_has_next(&it)) {
        free(list_next(&it));
    }

    list_delete(game->inactive_asteroids);
    free_ship_model();
}

#ifdef DEBUG
static void debug_controls(asteroids *game, App *app)
{
    if (app->keyboard.pressed[KEY_N]) {
        spawn_asteroid(game, 10, 10, rand() % 3);
    }

    if (app->keyboard.pressed[KEY_M]) {
        asteroid *a = list_pop(game->active_asteroids, 0);
        if (a) {
            list_append(game->inactive_asteroids, a);
        }
    }

    if (app->keyboard.pressed[KEY_T]) {
        transition_to_title(game);
    }

    if (app->keyboard.pressed[KEY_P]) {
        transition_to_play(game);
    }

    if (app->keyboard.pressed[KEY_G]) {
        transition_to_gameover(game);
    }

    if (app->keyboard.pressed[KEY_R]) {
        transition_to_reset(game);
    }

    if (app->keyboard.pressed[KEY_1]) {
        transition_to_test(game);
    }

    if (app->keyboard.pressed[KEY_H]) {
        transition_to_highscore(game);
    }
}
#endif //DEBUG

void clear_asteroids(asteroids *game)
{
    List_Iterator it = list_iterator(game->active_asteroids);
    asteroid *a;
    List *to_remove = list_new();
    while (list_has_next(&it)) {
        a = list_next(&it);
        list_append(to_remove, a);
    }

    it = list_iterator(to_remove);
    while (list_has_next(&it)) {
        a = list_next(&it);
        if (a) {
            list_remove(game->active_asteroids, a);
            list_append(game->inactive_asteroids, a);
        }
    }
    list_delete(to_remove);
}

int spawn_asteroid(asteroids *game, float x, float y, enum asteroid_type type)
{
    int success = 0;
    asteroid *a = list_pop(game->inactive_asteroids, 0);
    if (a) {
        asteroid_init(a, type);
        a->pos = new_vec2(x, y);
        list_append(game->active_asteroids, a);
        success = 1;
    }
    return success;
}

void update_asteroid_list(List *asteroids, float dt)
{
    List_Iterator it = list_iterator(asteroids);
    asteroid *a;
    while ((a = list_next(&it))) {
        asteroid_update(a, dt);
    }
}

void update_bullets(bullet_list *bullet_list, float dt)
{
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullet *b = &bullet_list->bullets[i];
        if (b->active_flag) {
            if (b->timer > BULLET_LIFETIME) {
                b->active_flag = INACTIVE;
                bullet_list->num_bullets--;
            } else {
                vec2 ds;
                vec2_mult(&ds, &b->vel, dt);
                vec2_add(&b->pos, &b->pos, &ds);
                b->timer += dt;
            }
        }
    }
}

explosion *find_inactive_explosion(explosion *expl_array, int size)
{
    explosion *e = NULL;
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!expl_array[i].active_flag) {
            e = &expl_array[i];
            break;
        }
    }
    return e;
}

void update_explosions(explosion *expl_array, float dt)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosion *e = &expl_array[i];
        if (e->active_flag) {
            explosion_update(e, dt);
        }
    }
}

void load_scores(asteroids *game, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("could not open highscore file %s\n", filename);
        exit(1);
    }

    // read the scores and save them to the datastruct
    for (int i = 0; i < 11; i++) {
        fscanf(fp, "%d %3s\n", &game->score_board[i].points, game->score_board[i].initials);
    }

    fclose(fp);
}

void save_scores(asteroids *game, char *filename)
{

}
