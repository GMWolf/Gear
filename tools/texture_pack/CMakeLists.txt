add_executable(texture_pack main.cpp TexturePacker.cpp TexturePacker.h)
target_link_libraries(texture_pack PUBLIC bfg_lyra stb nlohmann_json::nlohmann_json yaml-cpp)


function(add_texture_pack name directory width height)
    file(GLOB_RECURSE srcs
            CONFIGURE_DEPENDS
            "${directory}/*.png"
            )

    add_custom_command(
            COMMAND texture_pack ARGS -w ${width} -h ${height} -o ${name} ${CMAKE_CURRENT_LIST_DIR}/${directory}
            OUTPUT ${name}.json ${name}.png
            DEPENDS "${sprite_srcs}" texture_pack
            USES_TERMINAL
    )

    add_custom_target(${name}
            DEPENDS "${name}.json" "${name}.png")

endfunction()
