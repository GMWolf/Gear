set(CMAKE_CXX_STANDARD 17)

add_texture_pack(shmup_textures assets/sprites/sprites.yaml)
add_font(shmup_default_font assets/fonts/defaultFont.yaml)
add_shader(simple_textured src/shaders/simple_textured.vert src/shaders/simple_textured.frag)
add_shader(shd_font src/shaders/simple_textured.vert src/shaders/font.frag)


function(gear_project)
    cmake_parse_arguments(GEAR_PROJECT "" "EXECUTABLE" "ASSETS" ${ARGN})
    add_dependencies(${GEAR_PROJECT_EXECUTABLE} ${GEAR_PROJECT_ASSETS})


    set(ASSET_STRING "")
    foreach(asset ${GEAR_PROJECT_ASSETS})
        get_property(asset_type TARGET ${asset} PROPERTY GEAR_ASSET_TYPE)
        get_property(asset_path TARGET ${asset} PROPERTY GEAR_ASSET_PATH)

        list(APPEND ASSET_STRING ${asset_type} "," ${asset_path} "\n")
    endforeach()

endfunction()


add_executable(shmup src/main.cpp src/Collisions.h src/Collisions.cpp)
target_link_libraries(shmup PUBLIC gear)

gear_project(
        EXECUTABLE shmup
        ASSETS shmup_textures shmup_default_font simple_textured)