set(CMAKE_CXX_STANDARD 17)


add_texture_pack(gearhell_textures sprites 512 512)

add_executable(gearhell src/main.cpp)
target_link_libraries(gearhell PUBLIC gear)
add_dependencies(gearhell gearhell_textures)

