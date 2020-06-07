
add_executable(shmup src/main.cpp src/Collisions.h src/Collisions.cpp)
target_link_libraries(shmup PUBLIC gear)


add_texture_pack(shmup_textures assets/sprites/Sprites.tsx)