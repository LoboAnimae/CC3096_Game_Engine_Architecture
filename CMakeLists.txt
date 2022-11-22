cmake_minimum_required(VERSION 3.23)
project(PongECS)
set(CMAKE_CXX_STANDARD 23)

execute_process(
        COMMAND bash -c "cd make-build-debug; conan install ..; cd ..;"
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(PongECS src/main.cpp src/Game/Game.h src/Game/Game.cpp src/Scene/Scene.cpp src/Scene/Scene.h src/Entity/Entity.cpp src/Entity/Entity.h)

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS} SDL2main SDL2)
