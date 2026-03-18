cmake_minimum_required(VERSION 3.20)

project(IZG2026)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) 

add_subdirectory(libs/SDL-release-3.4.2)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)

