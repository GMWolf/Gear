add_executable(texture_pack main.cpp TexturePacker.cpp TexturePacker.h)
target_link_libraries(texture_pack PUBLIC bfg_lyra indica stb nlohmann_json::nlohmann_json)


function(add_texture_pack name directory width height)
    file(GLOB srcs
            CONFIGURE_DEPENDS
            "${directory}/*.png"
            )

    add_custom_command(
            COMMAND texture_pack ARGS -w ${width} -h ${height} -o sprites ${srcs}
            OUTPUT sprites.json sprites.png
            DEPENDS "${sprite_srcs}"
            USES_TERMINAL
    )

    add_custom_target(${name}
            DEPENDS "sprites.json" "sprites.png")

endfunction()
