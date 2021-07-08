#include "defs.h"

static Emitter particles;

static void test_update(asteroids *game, float dt)
{
    if (game->input[FIRE]) {
        emitter_start_emitting(&particles);
    }

    if (game->input[THRUST]) {
        emitter_stop_emitting(&particles);
    }

    emitter_update(&particles, dt);
}

static void test_render(asteroids *game)
{
    clear_screen();
    emitter_render(&particles);
    draw_text("test state", 0, 0, 0xffffff);
    if (particles.emit_flag) {
        draw_text("on", 0, 9, 0xffffff);
    } else {
        draw_text("off", 0, 9, 0xffffff);
    }
}

void transition_to_test(asteroids *game)
{
    default_state(&game->gamestate);
    game->gamestate.update = test_update;
    game->gamestate.render = test_render;
    emitter_init(&particles, 100, WIDTH / 2, HEIGHT / 2);
}
