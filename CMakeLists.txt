cmake_minimum_required(VERSION 3.6)
project(raycaster)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories(include
                    vendor)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES
    include/mathutils/mathutils.h
    include/mathutils/vec2.h
    include/resource_loader.h
    include/game.h
    include/raycaster.h
    include/color.h
    include/common.h
    include/coretimer.h
    include/graphics.h
    include/input.h
    include/iresource.h
    include/json_parser.h
    include/logger.h
    include/map.h
    include/player.h
    include/profile_timer.h
    include/world.h
    include/texture.h
    include/utils.h
    include/window.h
    vendor/picojson.h
    vendor/stb_image.h
    src/mathutils/mathutils.cpp
    src/mathutils/vec2.cpp
    src/resource_loader.cpp
    src/game.cpp
    src/raycaster.cpp
    src/coretimer.cpp
    src/graphics.cpp
    src/input.cpp
    src/main.cpp
    src/map.cpp
    src/player.cpp
    src/profile_timer.cpp
    src/world.cpp
    src/texture.cpp
    src/window.cpp)

add_executable(raycaster ${SOURCE_FILES})
target_link_libraries(raycaster ${SDL2_LIBRARY})
