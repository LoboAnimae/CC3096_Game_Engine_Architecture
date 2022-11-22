//
// Created by yagdrassyl on 11/15/22.
//
#include "Game.h"
#include <entt/entt.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

namespace pong_ecs {

    struct MetadataComponent {
        std::string name;
    };
    struct PositionComponent {
        int x;
        int y;
    };

    struct VelocityComponent {
        int x;
        int y;
    };

    auto playerVelocity = 100;

    struct CubeComponent {
        int width;
        int height;
    };

    struct ColliderComponent {
        bool canCollide;
    };


    struct InputComponent {
        SDL_KeyCode up;
        SDL_KeyCode down;
    };


    /**
     * A point in space
     */
    struct Point {
        int x;
        int y;
        int vector;

        Point(int p_x, int p_y) {
            x = p_x;
            y = p_y;
            vector = x + y;
        }

        bool isEqual(Point *other) const {
            auto otherX = other->x;
            auto otherY = other->y;

            return x == otherX && y == otherY;
        }
    };


    void createPlayerEntities(entt::registry &reg, int screenHeight, int screenWidth) {
        auto player1 = reg.create();
        auto player2 = reg.create();

        auto paddleWidth = 20;
        auto paddleHeight = 100;
        auto padding = paddleWidth / 2;

        auto paddleYPosition = (screenHeight / 2) - (paddleHeight / 2);
        reg.emplace<PositionComponent>(player1, PositionComponent({padding, paddleYPosition}));
        reg.emplace<PositionComponent>(player2,
                                       PositionComponent({screenWidth - paddleWidth - padding, paddleYPosition}));
        reg.emplace<CubeComponent>(player1, CubeComponent({paddleWidth, paddleHeight}));
        reg.emplace<CubeComponent>(player2, CubeComponent({paddleWidth, paddleHeight}));

        reg.emplace<InputComponent>(player1, InputComponent{SDLK_w, SDLK_s});
        reg.emplace<InputComponent>(player2, InputComponent{SDLK_UP, SDLK_DOWN});
        reg.emplace<MetadataComponent>(player1, MetadataComponent{"Player1"});
        reg.emplace<MetadataComponent>(player2, MetadataComponent{"Player2"});

        reg.emplace<ColliderComponent>(player1, ColliderComponent{});
        reg.emplace<ColliderComponent>(player2, ColliderComponent{});
    }

    void createBallEntity(entt::registry &reg, int screenHeight, int screenWidth) {
        auto ball = reg.create();

        auto ballWidth = 15;
        auto ballHeight = 15;

//        auto xPos = 20;
        auto xPos = screenWidth / 2 - ballWidth / 2;
        auto yPos = screenHeight / 2 - ballHeight / 2;
//        auto ballVelocity = 0;
        auto ballVelocity = 150;
        reg.emplace<PositionComponent>(ball, PositionComponent({xPos, yPos}));
        reg.emplace<VelocityComponent>(ball, VelocityComponent({ballVelocity, ballVelocity}));
        reg.emplace<CubeComponent>(ball, CubeComponent({ballWidth, ballHeight}));
        reg.emplace<ColliderComponent>(ball, ColliderComponent{});
        reg.emplace<MetadataComponent>(ball, MetadataComponent{"Ball"});
    }

    void cubeRenderSystem(SDL_Renderer *renderer, entt::registry &reg) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
        auto view = reg.view<CubeComponent, PositionComponent>();
        for (const auto entity: view) {
            auto position = view.get<PositionComponent>(entity);
            auto dimensions = view.get<CubeComponent>(entity);

            SDL_Rect rect = {position.x, position.y, dimensions.width, dimensions.height};
            SDL_RenderFillRect(renderer, &rect);
        }
    }


    bool isColliding(PositionComponent position, CubeComponent cube, Point point) {
        auto rightOfLeft = position.x < point.x;
        auto leftOfRight = position.x + cube.width > point.x;
        auto underTop = position.y < point.y;
        auto overBottom = position.y + cube.height > point.y;
        return rightOfLeft && leftOfRight && underTop && overBottom;
    }

    void ballMovementSystem(entt::registry &reg, int screenWidth, int screenHeight, float deltaTime) {
        auto view = reg.view<VelocityComponent, PositionComponent, CubeComponent, ColliderComponent>();
        for (const auto entity: view) {
            auto input = reg.try_get<InputComponent>(entity);
            if (input != nullptr) continue; // Only the ball
//            std::cout << "Ball found" << std::endl;
            auto &velocity = reg.get<VelocityComponent>(entity);
            auto &position = reg.get<PositionComponent>(entity);
            auto &cube = reg.get<CubeComponent>(entity);

            auto collidersView = reg.view<ColliderComponent, PositionComponent, CubeComponent>();
            auto ballCollider = reg.get<ColliderComponent>(entity);
//            auto ballCollider = reg.get<Collider>(entity);
            for (const auto collidingEntity: collidersView) {
                auto entityPosition = reg.get<PositionComponent>(collidingEntity);
                auto entityCube = reg.get<CubeComponent>(collidingEntity);
                if (entityCube.width == cube.width && entityCube.height == cube.height) continue;


                /* ______
                 * |    |
                 * |  .__L__
                 * |  | |  |
                 *  --L____|        .   -> To the right of left
                 *                      -> To the left of right
                 *                      -> under top
                 *                      -> over bottom
                 */

                auto topLeft = Point{position.x, position.y};
                auto topRight = Point{position.x + cube.width, position.y};
                auto bottomLeft = Point{position.x, position.y + cube.height};
                auto bottomRight = Point{position.x + cube.width, position.y + cube.height};
                Point points[] = {topLeft, topRight, bottomLeft, bottomRight};
                auto topLeftCollides = isColliding(entityPosition, entityCube, topLeft);
                auto topRightCollides = isColliding(entityPosition, entityCube, topRight);
                auto bottomLeftCollides = isColliding(entityPosition, entityCube, bottomLeft);
                auto bottomRightCollides = isColliding(entityPosition, entityCube, bottomRight);
                if (topLeftCollides || topRightCollides || bottomLeftCollides || bottomRightCollides) {
                    velocity.x *= -1.2;
                    break;
                }
            }


            auto nextXPos = position.x + velocity.x * deltaTime;
            auto nextYPos = position.y + velocity.y * deltaTime;

            if (nextXPos + cube.width > screenWidth) {
                std::cout << "Player 1 wins!" << std::endl;
                exit(1);
            } else if (nextXPos <= 0) {
                std::cout << "Player 2 wins!" << std::endl;
                exit(1);
            }
            if (nextYPos + cube.height > screenHeight || nextYPos <= 0) {
                velocity.y *= -1.2;
                nextYPos = position.y + velocity.y * deltaTime;
            }

            position.x = nextXPos;
            position.y = nextYPos;
        }
    }


    bool Game::init(const char *p_title, int p_screenWidth, int p_screenHeight) {
        if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
            window = SDL_CreateWindow(p_title, 0, 0, p_screenWidth, p_screenHeight, 0);
            renderer = SDL_CreateRenderer(window, -1, 0);
            screenWidth = p_screenWidth;
            screenHeight = p_screenHeight;
            isRunning = true;
        }
        return isRunning;
    }

    bool Game::setup() {
        createPlayerEntities(reg, screenHeight, screenWidth);
        createBallEntity(reg, screenHeight, screenWidth);
        return true;
    }

    void Game::frameStart() {
        frameStartTimestamp = SDL_GetTicks();
        deltaTime = frameEndTimestamp ?
                    (frameStartTimestamp - frameEndTimestamp) / 1000.0f :
                    0;

    }

    void Game::frameEnd() {
        frameEndTimestamp = SDL_GetTicks();
        auto actualFrameDuration = frameEndTimestamp - frameStartTimestamp;
        if (actualFrameDuration < frameDuration) {
            SDL_Delay(frameDuration - actualFrameDuration);
        }
    }

    void Game::handleEvents() {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
                return;
            }
            auto view = reg.view<InputComponent, PositionComponent, CubeComponent, MetadataComponent>();
            for (const auto entity: view) {
                auto metadata = reg.get<MetadataComponent>(entity);
                auto input = reg.get<InputComponent>(entity);
                auto &position = reg.get<PositionComponent>(entity);
                auto cube = reg.get<CubeComponent>(entity);
                auto pressedKey = event.key.keysym.sym;
                auto moveBy = playerVelocity * deltaTime;
                if (pressedKey == input.up && position.y - moveBy > 0) {
                    position.y -= moveBy;
                } else if (pressedKey == input.down && position.y + moveBy + cube.height < screenHeight) {
                    position.y += moveBy;
                }
            }
        }
    }

    void Game::update() {
        ballMovementSystem(reg, screenWidth, screenHeight, deltaTime);

//        collisionSystem(reg, deltaTime, screenHeight);
    }

    void Game::render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
        SDL_RenderClear(renderer);
        cubeRenderSystem(renderer, reg);
        SDL_RenderPresent(renderer);
    }

    void Game::clean() {
        SDL_DestroyWindow(window);
    }

    bool Game::running() {
        return isRunning;
    }

    Game::Game() {
        window = nullptr;
        renderer = nullptr;


        FPS = 30;
        frameStartTimestamp = (Uint64) NULL;
        frameEndTimestamp = (Uint64) NULL;
        frameDuration = (1.0f / (float) FPS) * 1000.0f;
        SDL_RenderSetLogicalSize(renderer, 160, 144);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    }

}

