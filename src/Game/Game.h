//
// Created by yagdrassyl on 11/15/22.
//

#ifndef PONGECS_GAME_H
#define PONGECS_GAME_H

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

namespace pong_ecs {
    class Game {

        bool isRunning = false;
        SDL_Window *window;
        SDL_Renderer *renderer;
        int FPS;
        Uint32 frameStartTimestamp;
        Uint32 frameEndTimestamp;
        Uint32 frameDuration;
        float deltaTime;
        int screenWidth;
        int screenHeight;
        entt::entity player1;
        entt::entity player2;


    public:
        entt::registry reg;

        Game();

        /**
         * Initializes the game. Generally works with windows
         * @param title The title of the window
         * @param screenWidth The width of the window
         * @param screenHeight The height of the window
         * @return True if everything is fine
         */
        bool init(const char *title, int screenWidth, int screenHeight);

        /**
         * Sets up the game
         * @return True if the setup runs as expected
         */
        bool setup();

        /**
         * Marks the start of a frame
         */
        void frameStart();

        /**
         * Handles the events before the update
         */
        void handleEvents();

        /**
         * Runs every frame before rendering
         */
        void update();

        /**
         * Renders stuff to the window
         */
        void render();

        /**
         * Marks the end of a frame
         */
        void frameEnd();

        /**
         * Cleans the memory
         */
        void clean();

        /**
         * Shows whether the game is running
         * @return True if running
         */
        bool running();


    };
}


#endif //PONGECS_GAME_H
