set(CMAKE_CXX_STANDARD 17)


add_texture_pack(shmup_textures sprites 512 512)

add_executable(shmup src/main.cpp)
target_link_libraries(shmup PUBLIC gear)
add_dependencies(shmup shmup_textures)