cmake_minimum_required(VERSION 3.15)
project(Gear)

set(CMAKE_CXX_STANDARD 17)

include(glfw.cmake)
include(stb.cmake)
add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
add_subdirectory(deps/indicators)
add_subdirectory(deps/Lyra)
add_subdirectory(gear)
add_subdirectory(tools/texture_pack)


add_executable(gear_test main.cpp)
target_link_libraries(gear_test PUBLIC gear)