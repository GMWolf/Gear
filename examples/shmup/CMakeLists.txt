set(CMAKE_CXX_STANDARD 17)

add_texture_pack(shmup_textures sprites/sprites.yaml)
add_font(shmup_default_font fonts/defaultFont.yaml)
add_shader(simple_textured src/shaders/simple_textured.vert src/shaders/simple_textured.frag)

add_executable(shmup src/main.cpp src/Collisions.h src/Collisions.cpp)
target_link_libraries(shmup PUBLIC gear)

add_dependencies(shmup shmup_textures)
add_dependencies(shmup simple_textured)
add_dependencies(shmup shmup_default_font)