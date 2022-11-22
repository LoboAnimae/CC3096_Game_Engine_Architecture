#include "Game/Game.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main() {
    auto *game = new pong_ecs::Game();

    game->init("Game", SCREEN_WIDTH, SCREEN_HEIGHT);
    game->setup();

    while (game->running()) {
        game->frameStart();
        game->handleEvents();
        game->update();
        game->render();
        game->frameEnd();
    }
    game->clean();
    return 0;
}
