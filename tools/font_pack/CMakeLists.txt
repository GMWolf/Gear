add_executable(font_pack main.cpp)
target_link_libraries(font_pack PUBLIC bfg_lyra stb nlohmann_json::nlohmann_json yaml-cpp)


function(add_font name input)

    add_custom_command(
            COMMAND font_pack ARGS -o ${name} ${CMAKE_CURRENT_LIST_DIR}/${input}
            OUTPUT ${name}.json ${name}.png
            DEPENDS "${input}" texture_pack
            USES_TERMINAL
    )

    add_custom_target(${name}
            SOURCES ${input}
            DEPENDS "${name}.json" "${name}.png")

    set_target_properties(${name} PROPERTIES
            GEAR_ASSET_PATH "${name}.json"
            GEAR_ASSET_TYPE "font")

endfunction()
