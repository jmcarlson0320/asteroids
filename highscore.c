#include "defs.h"
#include <stdio.h>

#define SCORES_FILENAME "highscores.data"
#define BUFFER_SIZE 256

static float timer_sec = 0.0f;

static void render_scoreboard(score scoreboard[], int num_scores)
{
    char scores[BUFFER_SIZE] = {'\0'};
    char *p = scores;

    for (int i = 0; i < NUM_HIGHSCORES; i++) {
        int num_chars = snprintf(p, BUFFER_SIZE - 1, "%d. %d %3s\n", i + 1, scoreboard[i].points, scoreboard[i].initials);
        p += num_chars;
    }

    draw_text(scores, WIDTH / 2 - (7 * 15 / 2), HEIGHT / 5, 0xffffff);
}

static void timer(asteroids *game)
{
    transition_to_title(game);
}

static void highscore_update(asteroids *game, float dt)
{
}

static void highscore_render(asteroids *game)
{
    clear_screen();
    draw_text("HIGHSCORES", WIDTH / 2 - (7 * 12 / 2), 40, 0xffffff);
    render_scoreboard(game->score_board, NUM_HIGHSCORES);
}

void transition_to_highscore(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.update = highscore_update;
    game->gamestate.render = highscore_render;

    load_scores(game, SCORES_FILENAME);
}
