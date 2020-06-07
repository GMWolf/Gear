
add_executable(shmup src/main.cpp src/Collisions.h src/Collisions.cpp)
target_link_libraries(shmup PUBLIC gear)


add_texture_pack(shmup_textures assets/sprites/Sprites.tsx)
add_shader_pack(textured_shd src/shaders/textured.yaml)
add_shader_pack(font_shd src/shaders/font.yaml)
add_shader_pack(prim_shd src/shaders/prim.yaml)

add_font_pack(shmup_default_font assets/fonts/defaultFont.yaml)

add_dependencies(shmup shmup_textures textured_shd font_shd prim_shd shmup_default_font)