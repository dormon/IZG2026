cmake_minimum_required(VERSION 3.20)

project(IZG2026)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE libs/glm-0.9.9.8)

option(SDL_SHARED "" OFF)
option(SDL_STATIC "" ON )
add_subdirectory(libs/SDL-release-3.4.2)

add_subdirectory(libs/geGL)

add_executable(${PROJECT_NAME} main.cpp bunny.hpp bunny.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3 geGL::geGL glm)

add_custom_target(run ./${PROJECT_NAME})

