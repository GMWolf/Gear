cmake_minimum_required(VERSION 3.15)
project(Gear)

set(CMAKE_CXX_STANDARD 17)

include(glfw.cmake)
include(stb.cmake)
include(imgui.cmake)
add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
add_subdirectory(deps/Lyra)
add_subdirectory(deps/tinyxml2)

set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_INSTALL OFF CACHE BOOL "" FORCE)
set(YAML_MSVC_SHARED_RT OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/yaml-cpp)

set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
set(JSON_Install OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/json)

define_property(TARGET PROPERTY GEAR_ASSET_TYPE
        BRIEF_DOCS "Defines the type of asset this target builds"
        FULL_DOCS "Defines the type of asset this target builds")

define_property(TARGET PROPERTY GEAR_ASSET_PATH
        BRIEF_DOCS "Defines the asset path to be loaded by the game"
        FULL_DOCS "Defines the asset path to be loaded by the game")

add_subdirectory(gear)
add_subdirectory(tools/texture_pack)
add_subdirectory(tools/shader_pack)
add_subdirectory(tools/font_pack)

OPTION(GEAR_BUILD_EXAMPLES "Build the examples" ON)

if(GEAR_BUILD_EXAMPLES)

add_subdirectory(examples/shmup)

endif()