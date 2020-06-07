
add_executable(tileset_packer shader_packer.cpp)
target_link_libraries(tileset_packer PUBLIC yaml-cpp stb tinyxml2)
target_include_directories(tileset_packer PUBLIC ../../)

function(add_tileset_pack name input)
    add_custom_command(
            COMMAND tileset_pack ARGS ${CMAKE_CURRENT_LIST_DIR}/${input} ${input}.bin
            OUTPUT ${input}.bin
            DEPENDS "${input}" tileset_packer
            USES_TERMINAL
    )

    add_custom_target(${name}
            DEPENDS "${input}.bin")
endfunction()