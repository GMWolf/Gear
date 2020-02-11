cmake_minimum_required(VERSION 3.15)
project(Gear)

set(CMAKE_CXX_STANDARD 17)

include(glfw.cmake)
include(stb.cmake)
add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
add_subdirectory(deps/Lyra)

set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(JSON_Install OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/json)

add_subdirectory(gear)
add_subdirectory(tools/texture_pack)
add_subdirectory(tools/shader_pack)


add_subdirectory(examples/shmup)

add_executable(gear_test main.cpp)
target_link_libraries(gear_test PUBLIC gear)

