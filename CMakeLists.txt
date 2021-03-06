cmake_minimum_required(VERSION 3.15)
project(Gear)


set(CMAKE_CXX_STANDARD 20)

include(cmake/glfw.cmake)
include(cmake/stb.cmake)
include(cmake/imgui.cmake)
include(cmake/tinygltf.cmake)

add_subdirectory(deps/glm)
add_subdirectory(deps/Lyra)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/tinyxml2)

add_library(cpp-base64 deps/cpp-base64/base64.h deps/cpp-base64/base64.cpp)
target_include_directories(cpp-base64 PUBLIC deps/cpp-base64)

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

add_subdirectory(deps/lz4/contrib/cmake_unofficial)

add_subdirectory(deps/zstd/build/cmake)

#add_subdirectory(deps/boost-cmake)

add_subdirectory(deps/freetype)

add_subdirectory(deps/spirv-headers)
add_subdirectory(deps/spirv-tools)
add_subdirectory(deps/glslang)
set(SHADERC_SKIP_TESTS ON CACHE BOOL "" FORCE)
add_subdirectory(deps/shaderc)
add_subdirectory(deps/spirv-cross)

set(CUTTLEFISH_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/Cuttlefish)
add_subdirectory(deps/tinygltf)
add_subdirectory(deps/meshoptimizer)
include(cmake/remotery.cmake)

add_subdirectory(gear)

add_subdirectory(tools)

add_subdirectory(examples/shmup)
add_subdirectory(examples/breakout)
add_subdirectory(examples/3d)