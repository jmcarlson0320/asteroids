#include "defs.h"

static int num_letters = 0;
static char initials[4] = {'\0'};
static char current_letter = 0;

static void clear_initials()
{
    for (int i = 0; i < 4; i++) {
        initials[i] = '\0';
    }
}

static void initials_entered(asteroids *game)
{
    transition_to_title(game);
}

static void gameover_update(asteroids *game, float dt)
{
    if (game->input[NEXT_LETTER]) {
        initials[current_letter]++;
        if (initials[current_letter] > 'Z') {
            initials[current_letter] = 'A';
        }
    }

    if (game->input[PREV_LETTER]) {
        initials[current_letter]--;
        if (initials[current_letter] < 'A') {
            initials[current_letter] = 'Z';
        }
    }

    if (game->input[FIRE]) {
        current_letter++;
        if (current_letter >= 3) {
            transition_to_title(game);
        }
        initials[current_letter] = 'A';
    }
}

static void gameover_render(asteroids *game)
{
    draw_text("gameover state", 10, 10, 0xff0000);
    draw_text(initials, 20, 20, 0xffffff);
}

void transition_to_gameover(asteroids *game)
{
    game->update = gameover_update;
    game->render = gameover_render;
    clear_initials();
    current_letter = 0;
    initials[current_letter] = 'A';
}
