#include "defs.h"
#include <stdio.h>

#define SCORES_FILENAME "highscores.data"
#define BUFFER_SIZE 256

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

static void highscore_update(asteroids *game, float dt)
{
}

static void highscore_render(asteroids *game)
{
    draw_text("HIGHSCORES", WIDTH / 2 - (7 * 12 / 2), 40, 0xffffff);
    render_scoreboard(game->score_board, NUM_HIGHSCORES);
}

void transition_to_highscore(asteroids *game)
{
    game->update = highscore_update;
    game->render = highscore_render;
    game->state = HIGHSCORE;

    load_scores(game, SCORES_FILENAME);
}
