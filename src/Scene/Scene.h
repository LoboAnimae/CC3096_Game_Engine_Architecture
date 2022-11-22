//
// Created by yagdrassyl on 19/11/22.
//

#ifndef PONGECS_SCENE_H
#define PONGECS_SCENE_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <string>
#include <entt/entt.hpp>

namespace pong_ecs {

//    class Entity;
//
//    class SetupSystem;
//
//    class InputSystem;
//
//    class UpdateSystem;
//
//    class RenderSystem;
//
//    class Scene {
//    private:
//        std::vector<SetupSystem *> setupSystems;
//        std::vector<InputSystem *> inputSystems;
//        std::vector<UpdateSystem *> updateSystems;
//        std::vector<RenderSystem *> renderSystems;
//
//    public:
//        Scene(const std::string &);
//
//        ~Scene();
//
//        entt::registry registry;
//        Entity *mainCamera;
//        Entity *player;
//
//        Entity createEntity(const std::string &name = "GENERIC", int x = 0, int y = 0);
//
//        void addSetupSystem(SetupSystem *newSystem);
//
//        void addInputSystem(InputSystem *newSystem);
//
//        void addUpdateSystem(UpdateSystem *newSystem);
//
//        void addRenderSystem(RenderSystem *newSystem);
//
//        void setup();
//
//        void update(double deltaTime);
//
//        void input(SDL_Event event);
//
//        void render(SDL_Renderer *renderer);
//
//    };

} // pong_ecs

#endif //PONGECS_SCENE_H
